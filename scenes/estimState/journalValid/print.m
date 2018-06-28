clear 
close all
clc

gtREAD= dlmread('groundTruth_BIF_SIDE');
n2dREAD= dlmread('noisyObs_BIF_SIDE');
P=[2281.11 -2.34947e-13 -400 273.957;-3.07529e-13 -2281.11 -300 -6.35711;-9.39116e-17 1.11022e-16 -1 0.317505];
stateREAD= dlmread('matlab/print_state_BIF_SIDE_2');

gt=gtREAD(:,2:end);
n2d=n2dREAD(:,2:end);

pos=cat(2,stateREAD(:,1:3),stateREAD(:,7:9),stateREAD(:,13:15),stateREAD(:,19:21),stateREAD(:,25:27),stateREAD(:,31:33),...
    stateREAD(:,37:39),stateREAD(:,43:45),stateREAD(:,49:51),stateREAD(:,55:57));

tip_gt=gt(:,1:3);
tip_filter=pos(:,1:3);

ms=1230;
A=ones(ms,2);
RMSE=ones(ms,1);
for i =1:ms
   A(i,1)=norm(tip_gt(i,:));
   A(i,2)=norm(tip_filter(i,:));
  RMSE(i)= sqrt(mean((A(i,1) - A(i,2)).^2));
 
end

pos2d=ones(ms,20);
gt2d=ones(ms,20);


  for i =1:ms
      rx = P(1,1) * pos(i,1) + P(1,2) * pos(i,2) + P(1,3) * pos(i,3) + P(1,4);
      ry = P(2,1) * pos(i,1) + P(2,2) * pos(i,2) + P(2,3) * pos(i,3) + P(2,4);
      rz = P(3,1) * pos(i,1) + P(3,2) * pos(i,2) + P(3,3) * pos(i,3) + P(3,4);
      pos2d(i,1)=rx* (1.0/rz);
      pos2d(i,2)=ry* (1.0/rz);    
            
      grx = P(1,1) * gt(i,1) + P(1,2) * gt(i,2) + P(1,3) * gt(i,3) + P(1,4);
      gry = P(2,1) * gt(i,1) + P(2,2) * gt(i,2) + P(2,3) * gt(i,3) + P(2,4);
      grz = P(3,1) * gt(i,1) + P(3,2) * gt(i,2) + P(3,3) * gt(i,3) + P(3,4);
      gt2d(i,1)=grx* (1.0/grz);
      gt2d(i,2)=gry* (1.0/grz);               
  end


    for i =1:ms
      rx = P(1,1) * pos(i,4) + P(1,2) * pos(i,5) + P(1,3) * pos(i,6) + P(1,4);
      ry = P(2,1) * pos(i,4) + P(2,2) * pos(i,5) + P(2,3) * pos(i,6) + P(2,4);
      rz = P(3,1) * pos(i,4) + P(3,2) * pos(i,5) + P(3,3) * pos(i,6) + P(3,4);
      pos2d(i,3)=rx* (1.0/rz);
      pos2d(i,4)=ry* (1.0/rz);    
      
      posx = P(1,1) * gt(i,4) + P(1,2) * gt(i,5) + P(1,3) * gt(i,6) + P(1,4);
      gry = P(2,1) * gt(i,4) + P(2,2) * gt(i,5) + P(2,3) * gt(i,6) + P(2,4);
      grz = P(3,1) * gt(i,4) + P(3,2) * gt(i,5) + P(3,3) * gt(i,6) + P(3,4);
      gt2d(i,3)=grx* (1.0/grz);
      gt2d(i,4)=gry* (1.0/grz);               
    end
  
    
      for i =1:ms
      rx = P(1,1) * pos(i,7) + P(1,2) * pos(i,8) + P(1,3) * pos(i,9) + P(1,4);
      ry = P(2,1) * pos(i,7) + P(2,2) * pos(i,8) + P(2,3) * pos(i,9) + P(2,4);
      rz = P(3,1) * pos(i,7) + P(3,2) * pos(i,8) + P(3,3) * pos(i,9) + P(3,4);
      pos2d(i,5)=rx* (1.0/rz);
      pos2d(i,6)=ry* (1.0/rz);     
      grx = P(1,1) * gt(i,7) + P(1,2) * gt(i,8) + P(1,3) * gt(i,9) + P(1,4);
      gry = P(2,1) * gt(i,7) + P(2,2) * gt(i,8) + P(2,3) * gt(i,9) + P(2,4);
      grz = P(3,1) * gt(i,7) + P(3,2) * gt(i,8) + P(3,3) * gt(i,9) + P(3,4);
      gt2d(i,5)=grx* (1.0/grz);
      gt2d(i,6)=gry* (1.0/grz);               
      end
  
      
      for i =1:ms
      rx = P(1,1) * pos(i,10) + P(1,2) * pos(i,11) + P(1,3) * pos(i,12) + P(1,4);
      ry = P(2,1) * pos(i,10) + P(2,2) * pos(i,11) + P(2,3) * pos(i,12) + P(2,4);
      rz = P(3,1) * pos(i,10) + P(3,2) * pos(i,11) + P(3,3) * pos(i,12) + P(3,4);
      pos2d(i,7)=rx* (1.0/rz);
      pos2d(i,8)=ry* (1.0/rz);   
      
      grx = P(1,1) * gt(i,10) + P(1,2) * gt(i,11) + P(1,3) * gt(i,12) + P(1,4);
      gry = P(2,1) * gt(i,10) + P(2,2) * gt(i,11) + P(2,3) * gt(i,12) + P(2,4);
      grz = P(3,1) * gt(i,10) + P(3,2) * gt(i,11) + P(3,3) * gt(i,12) + P(3,4);
      gt2d(i,7)=grx* (1.0/grz);
      gt2d(i,8)=gry* (1.0/grz);               
      end
  
        
      for i =1:ms
      rx = P(1,1) * pos(i,13) + P(1,2) * pos(i,14) + P(1,3) * pos(i,15) + P(1,4);
      ry = P(2,1) * pos(i,13) + P(2,2) * pos(i,14) + P(2,3) * pos(i,15) + P(2,4);
      rz = P(3,1) * pos(i,13) + P(3,2) * pos(i,14) + P(3,3) * pos(i,15) + P(3,4);
      pos2d(i,9)=rx* (1.0/rz);
      pos2d(i,10)=ry* (1.0/rz);
      grx = P(1,1) * gt(i,13) + P(1,2) * gt(i,14) + P(1,3) * gt(i,15) + P(1,4);
      gry = P(2,1) * gt(i,13) + P(2,2) * gt(i,14) + P(2,3) * gt(i,15) + P(2,4);
      grz = P(3,1) * gt(i,13) + P(3,2) * gt(i,14) + P(3,3) * gt(i,15) + P(3,4);
      gt2d(i,9)=grx* (1.0/grz);
      gt2d(i,10)=gry* (1.0/grz);               
      end
  
          
          
      for i =1:ms
      rx = P(1,1) * pos(i,16) + P(1,2) * pos(i,17) + P(1,3) * pos(i,18) + P(1,4);
      ry = P(2,1) * pos(i,16) + P(2,2) * pos(i,17) + P(2,3) * pos(i,18) + P(2,4);
      rz = P(3,1) * pos(i,16) + P(3,2) * pos(i,17) + P(3,3) * pos(i,18) + P(3,4);
      pos2d(i,11)=rx* (1.0/rz);
      pos2d(i,12)=ry* (1.0/rz); 
      grx = P(1,1) * gt(i,16) + P(1,2) * gt(i,17) + P(1,3) * gt(i,18) + P(1,4);
      gry = P(2,1) * gt(i,16) + P(2,2) * gt(i,17) + P(2,3) * gt(i,18) + P(2,4);
      grz = P(3,1) * gt(i,16) + P(3,2) * gt(i,17) + P(3,3) * gt(i,18) + P(3,4);
      gt2d(i,11)=grx* (1.0/grz);
      gt2d(i,12)=gry* (1.0/grz);               
       end
  
            
      for i =1:ms
      rx = P(1,1) * pos(i,19) + P(1,2) * pos(i,20) + P(1,3) * pos(i,21) + P(1,4);
      ry = P(2,1) * pos(i,19) + P(2,2) * pos(i,20) + P(2,3) * pos(i,21) + P(2,4);
      rz = P(3,1) * pos(i,19) + P(3,2) * pos(i,20) + P(3,3) * pos(i,21) + P(3,4);
      pos2d(i,13)=rx* (1.0/rz);
      pos2d(i,14)=ry* (1.0/rz);  
      grx = P(1,1) * gt(i,19) + P(1,2) * gt(i,20) + P(1,3) * gt(i,21) + P(1,4);
      gry = P(2,1) * gt(i,19) + P(2,2) * gt(i,20) + P(2,3) * gt(i,21) + P(2,4);
      grz = P(3,1) * gt(i,19) + P(3,2) * gt(i,20) + P(3,3) * gt(i,21) + P(3,4);
      gt2d(i,13)=grx* (1.0/grz);
      gt2d(i,14)=gry* (1.0/grz);               
      end
  
  
              
 for i =1:ms
      rx = P(1,1) * pos(i,22) + P(1,2) * pos(i,23) + P(1,3) * pos(i,24) + P(1,4);
      ry = P(2,1) * pos(i,22) + P(2,2) * pos(i,23) + P(2,3) * pos(i,24) + P(2,4);
      rz = P(3,1) * pos(i,22) + P(3,2) * pos(i,23) + P(3,3) * pos(i,24) + P(3,4);
      pos2d(i,15)=rx* (1.0/rz);
      pos2d(i,16)=ry* (1.0/rz);       
      grx = P(1,1) * gt(i,22) + P(1,2) * gt(i,23) + P(1,3) * gt(i,24) + P(1,4);
      gry = P(2,1) * gt(i,22) + P(2,2) * gt(i,23) + P(2,3) * gt(i,24) + P(2,4);
      grz = P(3,1) * gt(i,22) + P(3,2) * gt(i,23) + P(3,3) * gt(i,24) + P(3,4);
      gt2d(i,15)=grx* (1.0/grz);
      gt2d(i,16)=gry* (1.0/grz);               
 end
  
  for i =1:ms
      rx = P(1,1) * pos(i,25) + P(1,2) * pos(i,26) + P(1,3) * pos(i,27) + P(1,4);
      ry = P(2,1) * pos(i,25) + P(2,2) * pos(i,26) + P(2,3) * pos(i,27) + P(2,4);
      rz = P(3,1) * pos(i,25) + P(3,2) * pos(i,26) + P(3,3) * pos(i,27) + P(3,4);
      pos2d(i,17)=rx* (1.0/rz);
      pos2d(i,18)=ry* (1.0/rz);  
      grx = P(1,1) * gt(i,25) + P(1,2) * gt(i,26) + P(1,3) * gt(i,27) + P(1,4);
      gry = P(2,1) * gt(i,25) + P(2,2) * gt(i,26) + P(2,3) * gt(i,27) + P(2,4);
      grz = P(3,1) * gt(i,25) + P(3,2) * gt(i,26) + P(3,3) * gt(i,27) + P(3,4);
      gt2d(i,17)=grx* (1.0/grz);
      gt2d(i,18)=gry* (1.0/grz);               
  end 
  
tip_gt2d=gt2d(:,1:2);
tip_filter2d=pos2d(:,1:2);
A2d=ones(ms,2);
RMSE2d=ones(ms,1);
for i =1:ms
   A2d(i,1)=norm(tip_gt2d(i,:));
   A2d(i,2)=norm(tip_filter2d(i,:));
  RMSE2d(i)= sqrt(mean((A2d(i,1) - A2d(i,2)).^2));
 
end


%% HAUSDORF DISTANCE

hd=ones(ms,1);
hd2d=ones(ms,1);

for i=1:ms
hd2d(i) = HausdorffDist(gt2d(i,:),pos2d(i,:));
hd(i) = HausdorffDist(gt(i,:),pos(i,:));

end


SIDE_RMSE2d_2=RMSE2d;
SIDE_RMSE_2=RMSE;
SIDE_hd_2=hd;
SIDE_hd2d_2=hd2d;
save('SIDE_hd_2.mat','SIDE_hd_2');
save('SIDE_RMSE2d_2.mat','SIDE_RMSE2d_2');
save('SIDE_RMSE_2.mat','SIDE_RMSE_2');
save('SIDE_hd2d_2.mat','SIDE_hd2d_2');

