#ifndef AdaptativeUKFilterClassic_INL
#define AdaptativeUKFilterClassic_INL

#include "AdaptativeUKFilterClassic.h"
#include <sofa/core/visual/VisualParams.h>
#include <iostream>
#include <fstream>

namespace sofa
{
namespace component
{
namespace stochastic
{

template <class FilterType>
AdaptativeUKFilterClassic<FilterType>::AdaptativeUKFilterClassic()
    : Inherit()
    , d_exportPrefix( initData(&d_exportPrefix, "exportPrefix", "prefix for storing various quantities into files"))
    , d_filenameFinalState( initData(&d_filenameFinalState, "filenameFinalState", "output file name"))
    , d_draw(initData(&d_draw, false, "draw","Activation of draw"))
    , d_radius_draw( initData(&d_radius_draw, (double) 0.005,"radiusDraw", "radius of the spheres") )
    , d_MOnodes_draw( initData(&d_MOnodes_draw,(double) 1.0, "MOnodesDraw", "nodes of the mechanical object") )
    , d_lambda_0( initData(&d_lambda_0, "lambda_0", "nodes of the mechanical object") )
    , d_delta_0( initData(&d_delta_0, "delta_0", "nodes of the mechanical object") )
    , d_paramA( initData(&d_paramA,  "paramA", "nodes of the mechanical object") )
    , d_paramB( initData(&d_paramB, "paramB", "nodes of the mechanical object") )
    , d_chiSquared( initData(&d_chiSquared,"chiSquared", "nodes of the mechanical object") )


{

}

template <class FilterType>
void AdaptativeUKFilterClassic<FilterType>::computePerturbedStates()
{
    EVectorX xCol(stateSize);
    int currentPointID = 0;

    for (size_t i = 0; i < sigmaPointsNum; i++) {
        xCol = matXi.col(i);
        stateWrappers[0]->transformState(xCol, mechParams, &currentPointID);
        m_sigmaPointObservationIndexes[i] = currentPointID;
        matXi.col(i) = xCol;
    }
}

template <class FilterType>
void AdaptativeUKFilterClassic<FilterType>::computePrediction()
{
    hasObs = observationManager->hasObservation(this->actualTime);
    if (hasObs) {
        PRNS("Computing prediction, T= " << this->actualTime  << " ======");

        /// Computes Square Root with SVD
        EMatrixX matPsqrt(stateSize,stateSize);
        sqrtMat(stateCovar, matPsqrt);



        stateExp.fill(FilterType(0.0));
        stateExp = masterStateWrapper->getState();

        /// Computes X_{n}^{(i)-} sigma points
        for (size_t i = 0; i < sigmaPointsNum; i++) {
            matXi.col(i) = stateExp + matPsqrt * matI.row(i).transpose();
        }

        /// Propagate sigma points
        genMatXi=matXi.transpose();
        computePerturbedStates();

        /// Compute Predicted Mean
        stateExp.fill(FilterType(0.0));
        for (size_t i = 0; i < sigmaPointsNum; i++) {
            stateExp += matXi.col(i) * vecAlpha(i);
        }

        /// Compute Predicted Covariance
        EMatrixX matXiTrans= matXi.transpose();
        EMatrixX centeredPxx = matXiTrans.rowwise() - matXiTrans.colwise().mean();
        EMatrixX weightedCenteredPxx = centeredPxx.array().colwise() * vecAlphaVar.array();
        EMatrixX covPxx = (centeredPxx.adjoint() * weightedCenteredPxx);
        stateCovar = covPxx + modelCovar;

        masterStateWrapper->setState(stateExp, mechParams);
        predState=stateExp;

    }else{
        //        std::cout<<"\n HAS OBS: =" << hasObs << " COMPUTE ONLY PREDICTION" << std::endl;

        //        stateExp.fill(FilterType(0.0));
        //        stateExp = masterStateWrapper->getState();
        //        masterStateWrapper->lastApplyOperator(stateExp, mechParams);

        EMatrixX matPsqrt(stateSize,stateSize);
        sqrtMat(stateCovar, matPsqrt);



        stateExp.fill(FilterType(0.0));
        stateExp = masterStateWrapper->getState();

        /// Computes X_{n}^{(i)-} sigma points
        for (size_t i = 0; i < sigmaPointsNum; i++) {
            matXi.col(i) = stateExp + matPsqrt * matI.row(i).transpose();
        }

        /// Propagate sigma points
        genMatXi=matXi.transpose();
        computePerturbedStates();

        /// Compute Predicted Mean
        stateExp.fill(FilterType(0.0));
        for (size_t i = 0; i < sigmaPointsNum; i++) {
            stateExp += matXi.col(i) * vecAlpha(i);
        }

        /// Compute Predicted Covariance
        EMatrixX matXiTrans= matXi.transpose();
        EMatrixX centeredPxx = matXiTrans.rowwise() - matXiTrans.colwise().mean();
        EMatrixX weightedCenteredPxx = centeredPxx.array().colwise() * vecAlphaVar.array();
        EMatrixX covPxx = (centeredPxx.adjoint() * weightedCenteredPxx);
        stateCovar = covPxx + modelCovar;

        masterStateWrapper->setState(stateExp, mechParams);

    }
}

template <class FilterType>
void AdaptativeUKFilterClassic<FilterType>::computeCorrection()
{

    if (hasObs) {
        //        std::cout<<"\n HAS OBS: =" << hasObs << " COMPUTE CORRECTION" << std::endl;

        PRNS("======= Computing correction, T= " << this->actualTime << " ======");
        EVectorX zCol(observationSize);
        matZmodel.resize(observationSize, sigmaPointsNum);
        predObsExp.resize(observationSize);
        predObsExp.fill(FilterType(0.0));

        /// Compute Predicted Observation
        for (size_t i = 0; i < sigmaPointsNum; i++) {
            observationManager->getPredictedObservation(m_sigmaPointObservationIndexes[i],  zCol);
            matZmodel.col(i) = zCol;
            predObsExp = predObsExp + zCol * vecAlpha(i);
        }

        /// Compute State-Observation Cross-Covariance
        EMatrixX matPxz(stateSize, observationSize);
        EMatrixX matPz(observationSize, observationSize);
        EMatrixX matS(observationSize, observationSize);
        EMatrixX pinvR(observationSize, observationSize);
        pseudoInverse(obsCovar,pinvR);
        EMatrixX pinvmatPz(observationSize, observationSize);


        EMatrixX matXiTrans= matXi.transpose();
        EMatrixX matZItrans = matZmodel.transpose();
        EMatrixX centeredCx = matXiTrans.rowwise() - matXiTrans.colwise().mean();
        EMatrixX centeredCz = matZItrans.rowwise() - matZItrans.colwise().mean();
        EMatrixX weightedCenteredCz = centeredCz.array().colwise() * vecAlphaVar.array();
        EMatrixX covPxz = (centeredCx.adjoint() * weightedCenteredCz);

        /// Compute Observation Covariance
        matPxz=covPxz;
        EMatrixX covPzz = (centeredCz.adjoint() * weightedCenteredCz);
        matPz = obsCovar + covPzz;
        pseudoInverse(matPz, pinvmatPz);

        /// Compute Kalman Gain
        EMatrixX matK(stateSize, observationSize);
        matK =matPxz*pinvmatPz;

        /// Compute Innovation
        EVectorX innovation(observationSize);
        observationManager->getInnovation(this->actualTime, predObsExp, innovation);

        /// Compute Final State and Final Covariance
        stateExp = stateExp + matK * innovation;
        stateCovar = stateCovar - matK*matPxz.transpose();

        masterStateWrapper->setState(stateExp, mechParams);


        /// ADAPTIVE ADJUSTMENT
        computeAdaptiveParameters(mu,epsilon,stateExp,predState);
        double delta;
        double lambda;
        double D =(phi - d_paramA.getValue()*d_chiSquared.getValue())*(1.0/phi);
        if (D> d_delta_0.getValue()) delta=D;
        else delta=d_delta_0.getValue();
        std::cout<<"delta_0    "<< d_delta_0.getValue()<< "   " <<  " D   " << D << "    " << " delta  " << delta <<std::endl;

        double L =(phi - d_paramB.getValue()*d_chiSquared.getValue())*(1.0/phi);
        if (L> d_lambda_0.getValue()) lambda=L;
        else lambda=d_lambda_0.getValue();
        std::cout<<"lambda_0    "<< d_lambda_0.getValue()<< "   " <<  " L   " << L << "    " << " lambda  " << lambda <<std::endl;

        phi=innovation.transpose()*pinvmatPz*innovation;
        std::cout<<"phi  "<< phi << std::endl;

        if(phi > d_chiSquared.getValue()){
            /// Update Noise Covariance Matrices ZHENG 2018
            std::cout<<"Update Noise Covariance Matrices" << std::endl;

            modelCovar=(1-lambda)*modelCovar +lambda*(matK*mu*mu.transpose()*matK.transpose());
            obsCovar=(1-delta)*obsCovar +delta*(epsilon*epsilon.transpose()+covPzz);


            /// Correct State Estimations
            matPz = obsCovar + covPzz;
            matK =matPxz*pinvmatPz;

            stateExp = stateExp + matK * innovation;
            stateCovar = stateCovar - matK*matPxz.transpose();
            masterStateWrapper->setState(stateExp, mechParams);


        }


    }

    /// Write Some File for Validation
    char nstepc[100];
    sprintf(nstepc, "%04lu", stepNumber);
    if (! exportPrefix.empty()) {
        std::string fileName = exportPrefix + "/cov_" + nstepc + ".txt";
        std::ofstream ofs;
        ofs.open(fileName.c_str(), std::ofstream::out);
        ofs << stateCovar << std::endl;
    }

    writeValidationPlot(d_filenameFinalState.getValue() ,stateExp);

}

template <class FilterType>
void AdaptativeUKFilterClassic<FilterType>::computeAdaptiveParameters( EVectorX& _mu,  EVectorX& _epsilon, EVectorX& _stateExp, EVectorX& _predState)
{

    EVectorX _realObs(observationSize);
    EVectorX _HpredState(observationSize);
    EVectorX _HstateExp(observationSize);

    observationManager->getRealObservation(this->actualTime, _realObs);
    observationManager->obsFunction(_predState, _HpredState);
    observationManager->obsFunction(_stateExp,  _HstateExp);

    _mu=_realObs - _HpredState; /// innovation
    _epsilon= _realObs - _HstateExp;

}


template <class FilterType>
void AdaptativeUKFilterClassic<FilterType>::init() {
    Inherit::init();
    assert(this->gnode);

    this->gnode->template get<StochasticStateWrapperBaseT<FilterType> >(&stateWrappers, this->getTags(), sofa::core::objectmodel::BaseContext::SearchDown);
    PRNS("found " << stateWrappers.size() << " state wrappers");
    masterStateWrapper=NULL;
    size_t numSlaveWrappers = 0;
    size_t numMasterWrappers = 0;
    numThreads = 0;
    for (size_t i = 0; i < stateWrappers.size(); i++) {
        stateWrappers[i]->setFilterKind(CLASSIC);
        if (stateWrappers[i]->isSlave()) {
            numSlaveWrappers++;
            PRNS("found stochastic state slave wrapper: " << stateWrappers[i]->getName());
        } else {
            masterStateWrapper = stateWrappers[i];
            numMasterWrappers++;
            PRNS("found stochastic state master wrapper: " << masterStateWrapper->getName());
        }
    }

    if (numMasterWrappers != 1) {
        PRNE("Wrong number of master wrappers: " << numMasterWrappers);
        return;
    }


    /// Init for Adaptive Tuning Parameters
    if (d_lambda_0.isSet()  == 0){
        serr << "No parameter *lambda_0* initialized for Adaptive Adjustment of Q" << sendl;
        return;
    }

    if (d_delta_0.isSet()  == 0 ){
        serr << "No parameter *delta_0* initialized for Adaptive Adjustment of R" << sendl;
        return;
    }
    if (d_paramA.isSet()  == 0 ){
        serr << "No parameter *paramA* initialized for Adaptive Adjustment of Q" << sendl;
        return;
    }
    if (d_paramB.isSet()  == 0 ){
        serr << "No *paramB* initialized for Adaptive Adjustment of R" << sendl;
        return;
    }
    if (d_chiSquared.isSet()  == 0 ){
        serr << "No *chiSquared* initialized for Adaptive Adjustment of R" << sendl;
        return;
    }

    mu.resize(observationSize);
    epsilon.resize(observationSize);

    if (numThreads > 0) {
        PRNS("number of slave wrappers: " << numThreads-1);
        /// slaves + master
    }
    numThreads=numSlaveWrappers+numMasterWrappers;

    this->gnode->get(observationManager, core::objectmodel::BaseContext::SearchDown);
    if (observationManager) {
        PRNS("found observation manager: " << observationManager->getName());
    } else
        PRNE("no observation manager found!");


    /// Init for Write Function
    exportPrefix  = d_exportPrefix.getFullPath();

    this->saveParam = false;
    if (!d_filenameFinalState.getValue().empty()) {
        std::ofstream paramFileFinalState(d_filenameFinalState.getValue().c_str());
        if (paramFileFinalState .is_open()) {
            this->saveParam = true;
            paramFileFinalState.close();
        }
    }
    m_omega= ((double) rand() / (RAND_MAX));


}

template <class FilterType>
void AdaptativeUKFilterClassic<FilterType>::bwdInit() {
    assert(masterStateWrapper);

    stateSize = masterStateWrapper->getStateSize();
    std::cout<< "[UKF] stateSize " << stateSize << std::endl;


    /// Initialize Observation's data
    if (!initialiseObservationsAtFirstStep.getValue()) {
        observationSize = this->observationManager->getObservationSize();
        PRNS("Observation size: " << observationSize);
        if (observationSize == 0) {
            PRNE("No observations available, cannot allocate the structures!");
        }
        obsCovar = observationManager->getErrorVariance();
    }


    /// Initialize Model's Error Covariance
    stateCovar = masterStateWrapper->getStateErrorVariance();
    modelCovar = masterStateWrapper->getModelErrorVariance();
    stateExp = masterStateWrapper->getState();

    /// Compute Sigma Points
    switch (this->m_sigmaTopology) {
    case STAR:
        computeStarSigmaPoints(matI);
        break;
    case SIMPLEX:
    default:
        computeSimplexSigmaPoints(matI);
    }

    sigmaPointsNum = matI.rows();
    matXi.resize(stateSize, sigmaPointsNum);
    genMatXi.resize( sigmaPointsNum,stateSize);

    /// Init State Observation Mapping for Sigma Points
    m_sigmaPointObservationIndexes.resize(sigmaPointsNum);

}

template <class FilterType>
void AdaptativeUKFilterClassic<FilterType>::initializeStep(const core::ExecParams* _params, const size_t _step) {
    Inherit::initializeStep(_params, _step);

    if (initialiseObservationsAtFirstStep.getValue()) {
        observationSize = this->observationManager->getObservationSize();
        PRNS("Observation size: " << observationSize);
        if (observationSize == 0) {
            PRNE("No observations available, cannot allocate the structures!");
        }
        obsCovar = observationManager->getErrorVariance();
        initialiseObservationsAtFirstStep.setValue(false);
    }

    for (size_t i = 0; i < stateWrappers.size(); i++)
        stateWrappers[i]->initializeStep(stepNumber);

    observationManager->initializeStep(stepNumber);
}

template <class FilterType>
void AdaptativeUKFilterClassic<FilterType>::stabilizeMatrix (EMatrixX& _initial, EMatrixX& _stabilized) {
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(_initial, Eigen::ComputeThinU | Eigen::ComputeThinV);
    const Eigen::JacobiSVD<Eigen::MatrixXd>::SingularValuesType singVals = svd.singularValues();
    Eigen::JacobiSVD<Eigen::MatrixXd>::SingularValuesType singValsStab = singVals;
    for (int i=0; i < singVals.rows(); i++ ){
        if ((singValsStab(i)*singValsStab(i))*(1.0/(singVals(0)*singVals(0)))< 1.0e-6) singValsStab(i)=0;
    }
    _stabilized= svd.matrixU()*singValsStab*svd.matrixV().transpose();

}

template <class FilterType>
void AdaptativeUKFilterClassic<FilterType>::pseudoInverse( EMatrixX& M,EMatrixX& pinvM) {
    double epsilon= 1e-15;
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(M, Eigen::ComputeThinU | Eigen::ComputeThinV);
    const Eigen::JacobiSVD<Eigen::MatrixXd>::SingularValuesType singVals = svd.singularValues();
    Eigen::JacobiSVD<Eigen::MatrixXd>::SingularValuesType invSingVals = singVals;
    for(int i=0; i<singVals.rows(); i++) {
        if(singVals(i)*singVals(i) <= epsilon*epsilon) invSingVals(i) = 0.0;
        else invSingVals(i) = 1.0 / invSingVals(i);
    }
    Eigen::MatrixXd S_inv = invSingVals.asDiagonal();
    pinvM = svd.matrixV()*S_inv* svd.matrixU().transpose();
}


template <class FilterType>
void AdaptativeUKFilterClassic<FilterType>::sqrtMat(EMatrixX& A, EMatrixX& sqrtA){
    double epsilon= 1e-15;
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(A, Eigen::ComputeThinU | Eigen::ComputeThinV);
    const Eigen::JacobiSVD<Eigen::MatrixXd>::SingularValuesType singVals = svd.singularValues();
    Eigen::JacobiSVD<Eigen::MatrixXd>::SingularValuesType sqrtSingVals = singVals;
    for(int i=0; i<singVals.rows(); i++) {
        if(singVals(i)*singVals(i) <= epsilon*epsilon) sqrtSingVals(i) = 0.0;
        else sqrtSingVals(i) = sqrt(sqrtSingVals(i));
    }
    Eigen::MatrixXd S_inv = sqrtSingVals.asDiagonal();
    sqrtA = svd.matrixV()*S_inv* svd.matrixU().transpose();

}


template <class FilterType>
void AdaptativeUKFilterClassic<FilterType>::writeValidationPlot (std::string filename ,EVectorX& state ){
    if (this->saveParam) {
        std::ofstream paramFile(filename.c_str(), std::ios::app);
        if (paramFile.is_open()) {
            paramFile << state.transpose() << "";
            paramFile << '\n';
            paramFile.close();
        }
    }
}

template <class FilterType>
void AdaptativeUKFilterClassic<FilterType>::computeSimplexSigmaPoints(EMatrixX& sigmaMat) {
    size_t p = stateSize;
    size_t r = stateSize + 1;

    EMatrixX workingMatrix = EMatrixX::Zero(p, r);

    Type scal, beta, sqrt_p;
    beta = Type(p) / Type(p+1);
    sqrt_p = sqrt(Type(p));
    scal = Type(1.0)/sqrt(Type(2) * beta);
    workingMatrix(0,0) = scal;
    workingMatrix(0,1) = -scal;

    for (size_t i = 1; i < p; i++) {
        scal = Type(1.0) / sqrt(beta * Type(i+1) * Type(i+2));

        for (size_t j = 0; j < i+1; j++)
            workingMatrix(i,j) = -scal;
        workingMatrix(i,i+1) = Type(i+1) * scal;
    }


    sigmaMat.resize(r,p);
    for (size_t i = 0; i < r; i++)
        sigmaMat.row(i) = workingMatrix.col(i) * sqrt_p;

    vecAlpha.resize(r);
    vecAlpha.fill(Type(1.0)/Type(r));
    alphaConstant = true;
    alpha = vecAlpha(0);

    alphaVar = (this->useUnbiasedVariance.getValue()) ? Type(1.0)/Type(r-1) : Type(1.0)/Type(r);
    vecAlphaVar.resize(r);
    vecAlphaVar.fill(alphaVar);
}

template <class FilterType>
void AdaptativeUKFilterClassic<FilterType>::computeStarSigmaPoints(EMatrixX& sigmaMat) {
    size_t p = stateSize;
    size_t r = 2 * stateSize + 1;

    EMatrixX workingMatrix = EMatrixX::Zero(p, r);

    Type lambda, sqrt_vec;
    lambda = this->lambdaScale.getValue();
    PRNS("lambda scale equals: " << lambda);
    sqrt_vec = sqrt(p + lambda);

    for (size_t j = 0; j < p; j++) {
        workingMatrix(j,j) = sqrt_vec;
    }
    for (size_t j = p; j < 2 * p; j++) {
        workingMatrix(2*p-j-1,j) = -sqrt_vec;
    }

    sigmaMat.resize(r,p);
    for (size_t i = 0; i < r; i++)
        sigmaMat.row(i) = workingMatrix.col(i);

    vecAlpha.resize(r);
    vecAlpha.fill(Type(1.0)/Type(2 * (p + lambda)));
    vecAlpha(2 * p) = Type(lambda) / Type(p + lambda);
    alphaConstant = false;
    alpha = vecAlpha(0);

    alphaVar = (this->useUnbiasedVariance.getValue()) ? Type(1.0)/Type(2 * (p + lambda) - 1) : Type(1.0)/Type(2 * (p + lambda));
    vecAlphaVar.resize(r);
    vecAlphaVar.fill(alphaVar);
    vecAlphaVar(2 * p) = Type(lambda) / Type(p + lambda);
}

template <class FilterType>
void AdaptativeUKFilterClassic<FilterType>::draw(const core::visual::VisualParams* vparams ) {
    if(d_draw.getValue()){
        if (vparams->displayFlags().getShowVisualModels()) {
            std::vector<std::vector<sofa::defaulttype::Vec3d>> predpoints;
            predpoints.resize( sigmaPointsNum );
            for(  std::vector<std::vector<sofa::defaulttype::Vec3d>>::iterator it = predpoints.begin(); it != predpoints.end(); ++it)
            {
                it->resize( d_MOnodes_draw.getValue() );
            }

            for(unsigned i=0; i < sigmaPointsNum; i++){
                EVectorX coll = genMatXi.row(i);

                for (unsigned j=0; j < d_MOnodes_draw.getValue(); j++){
                    for (unsigned k=0; k < 3; k++){
                        if  (masterStateWrapper->estimOnlyXYZ())
                            predpoints[i][j][k]=coll(3*j+k);
                        else
                            predpoints[i][j][k]=coll(6*j+k);
                    }
                }


                Vec4f color;

                switch (i) {
                case 0: color = Vec4f(1.0,0.0,0.0,1.0); break;
                case 1: color = Vec4f(0.0,1.0,0.0,1.0); break;
                case 2: color = Vec4f(0.0,0.0,1.0,1.0); break;
                default: color = Vec4f(0.5, 0.5, 0.5, 0.5);
                }
                helper::vector<double>  colorB;
                colorB.resize(this->stateSize);
                for(size_t i =0; i < colorB.size(); i++){

                    colorB[i]= ((double) rand() / (RAND_MAX)) ;
                }

                vparams->drawTool()->drawSpheres(predpoints[i],  d_radius_draw.getValue(), sofa::defaulttype::Vec<4, float>(m_omega,0.0f,0.0f,1.0f)); }
            //                if (d_MOnodes_draw.getValue()>=2)
            //                    vparams->drawTool()->drawLineStrip(predpoints[i],3.0,sofa::defaulttype::Vec<4, float>(color[i],0.5f,colorB[i],1.0f));            }

        }
    }



}

} // stochastic
} // component
} // sofa

#endif // AdaptativeUKFilterClassic_INL


/*matPxzB.fill(FilterType(0.0));
matPzB.fill(FilterType(0.0));

EVectorX vx(stateSize), z(observationSize), vz(observationSize);
vx.fill(FilterType(0.0));
vz.fill(FilterType(0.0));
z.fill(FilterType(0.0));
for (size_t i = 0; i < sigmaPointsNum; i++) {
   vx = matXi.col(i) - stateExp;
   z = matZmodel.col(i);
   vz = z - predObsExp;
   for (size_t x = 0; x < stateSize; x++)
       for (size_t y = 0; y < observationSize; y++)
           matPxzB(x,y) += vx(x)*vz(y);

   for (size_t x = 0; x < observationSize; x++)
       for (size_t y = 0; y < observationSize; y++)
           matPzB(x,y) += vz(x)*vz(y);
}
matPxzB = alphaVar * matPxzB;
matPzB = alphaVar * matPzB + obsCovar;*/

/*
    stateCovar.fill(FilterType(0.0));
    EVectorX tmpX(stateSize);
    tmpX.fill(FilterType(0.0));
    for (size_t i = 0; i < sigmaPointsNum; i++) {
       tmpX = matXi.col(i) - stateExp;
       for (size_t x = 0; x < stateSize; x++)
           for (size_t y = 0; y < stateSize; y++)
               stateCovar(x,y) += tmpX(x)*tmpX(y);
    }
    stateCovar=alphaVar*stateCovar;
}*/