stoch5=[[0.0007329753638186721, 0.0014071685535525641, -0.009550288639799119]; [0.0978338575012962, 0.03172264514227881, -0.011921728302182731]; [0.09837443277597366, 0.03166918823636855, -0.0018914261384813584]; [0.000945607156723703, 0.0015826914305037277, -0.0008567861115734216]; [0.08383981325959655, 0.029722665309863817, -0.011200410610832726]; [0.07023601872386222, 0.026922876235264674, -0.010546317621224727]; [0.0567626034701897, 0.023585209722173175, -0.010074787393592488]; [0.04257491907654858, 0.017548206920010426, -0.009437628977708029]; [0.029326921313302733, 0.00782521431697304, -0.009095259040872793]; [0.014815140181105363, 0.0034835835251750733, -0.008700199973919033]; [0.08432346504495765, 0.029627212264940323, -0.0011428357526324735]; [0.07058965779980868, 0.027083023675059707, -0.00041286102239576503]; [0.0567742608357527, 0.023147270064670697, -0.0003226751202111067]; [0.042732157083936455, 0.017806948029141005, -0.0003758968187616615]; [0.029430864188469682, 0.009420400306692106, -0.0006664899293196407]; [0.014722253175953246, 0.002353781264154949, -0.0006261257012287066]];
stoch10=[[0.0004538964908245719, 0.000833185084877605, -0.009623432684709868]; [0.09768281993448046, 0.03168590304680224, -0.012143418684682347]; [0.09819425785858842, 0.031515665263447866, -0.002106404333466809]; [0.0004950869916108952, 0.0007611785035618083, -0.000529327097333484]; [0.08368716920321696, 0.029688812147864573, -0.011459643513759082]; [0.07008033359469559, 0.026932671590878535, -0.010820390052313446]; [0.05657385185467184, 0.023776427568900166, -0.010318313608004394]; [0.042605253242849944, 0.017931061532421095, -0.009552282323530639]; [0.029260151168514718, 0.008673315874365284, -0.00914507990629455]; [0.014699602980608515, 0.0029221949096476445, -0.008899180397630206]; [0.08415572788550098, 0.029453340029660244, -0.001416084425122938]; [0.0704760831766176, 0.02680052050223808, -0.00076692542681557]; [0.05669024175834672, 0.023337043708350023, -0.000515386941716549]; [0.042749996096620974, 0.0184058620630282, -0.0003490162893502202]; [0.02924084925000915, 0.010767814772337944, -0.000548256770157625]; [0.014629246301007393, 0.0021906287079305857, -0.0006254493593060065]];

gen=[[1.2928169187976407e-06, 1.5878685103623063e-06, -0.009998786986269909]; [0.09686573183305255, 0.03174737870069703, -0.011493810392790843]; [0.09739544284067649, 0.0318299497684928, -0.0014690158637515173]; [8.60055702017156e-07, 9.84321784492881e-07, -1.1718451427230438e-06]; [0.08285473683828698, 0.029883415999501946, -0.010775516829783177]; [0.0692010457539142, 0.027346831960180777, -0.010111352747615245]; [0.05570131114449826, 0.024274247935603323, -0.009617270763151147]; [0.042080675967182755, 0.018832755781047046, -0.009153908448547922]; [0.02802513007895355, 0.011508646331961723, -0.009187065944597414]; [0.014286176109114293, 4.799565271132441e-06, -0.009997348295362744]; [0.08331516151208068, 0.029934758460685197, -0.0007136116804823488]; [0.06953171701537929, 0.027639650158606987, 1.8623296135680306e-05]; [0.05573111947491524, 0.024150245862575928, 0.00022338475597741698]; [0.04186855576203874, 0.01945762234833338, 0.00042724327189032623]; [0.027368979530154415, 0.013139968783311511, 0.00014491726782360948]; [0.014286401077153442, 5.2316240852000565e-06, -2.670891351388302e-06]];


n=length(gen);

for i=1:n
    nrm(i) = norm(stoch10(i) - gen(i));
end