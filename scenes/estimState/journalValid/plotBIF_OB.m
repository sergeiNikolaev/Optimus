%%PLOT TIP OB_RMSE 
% load('OB_RMSE2d_0.mat','OB_RMSE2d_0')
% clear all
% close all 
% clc

load OB_RMSE2d_0.mat
load OB_RMSE2d_1.mat   
load OB_RMSE2d_2.mat
load OB_RMSE2d_3.mat

load OB_RMSE_0.mat
load OB_RMSE_1.mat
load OB_RMSE_2.mat
load OB_RMSE_3.mat

load OB_hd_0.mat
load OB_hd_1.mat
load OB_hd_2.mat
load OB_hd_3.mat

load OB_hd2d_0.mat 
load OB_hd2d_1.mat 
load OB_hd2d_2.mat 
load OB_hd2d_3.mat 

figure
plot(OB_RMSE_0*1000)
hold on
plot(OB_RMSE_1*1000, 'Color',[1 0 0])
hold on
plot(OB_RMSE_2*1000, 'Color',[0 0.8 0])
hold on
plot(OB_RMSE_3*1000, 'Color',[1 0 1])
grid on
xlabel('h - [simulation step]') % x-axis label
ylabel('OB_RMSE [mm] ') % y-axis label
title( {'Root Mean Square Error'; ' 3D Ground Truth Tip - 3D  Filter Tip' }) ;

figure
plot(OB_RMSE2d_0)
hold on
plot(OB_RMSE2d_1, 'Color',[1 0 0]) 
hold on
plot(OB_RMSE2d_2, 'Color',[0 0.8 0])
hold on
plot(OB_RMSE2d_3, 'Color',[1 0 1])
grid on
xlabel('h - [simulation step]') % x-axis label
ylabel('OB_RMSE [pixel] ') % y-axis label
title( {'2D Root Mean Square Error'; ' 2D Ground Truth Tip - 2D  Filter Tip' }) ;


figure
plot(OB_hd_0*1000)
hold on
plot(OB_hd_1*1000, 'Color',[1 0 0])
hold on
plot(OB_hd_2*1000, 'Color',[0 0.8 0])
hold on
plot(OB_hd_3*1000, 'Color',[1 0 1])
grid on
xlabel('h - [simulation step]') % x-axis label
ylabel('Hausdorff [mm] ') % y-axis label
title( {'Hausdorff Distance'; 'Ground Truth Catheter- Filter Catheter' }) ;

figure
plot(OB_hd2d_0)
hold on
plot(OB_hd2d_1, 'Color',[1 0 0])
hold on
plot(OB_hd2d_2, 'Color',[0 0.8 0])
hold on
plot(OB_hd2d_3, 'Color',[1 0 1])
grid on
xlabel('h - [simulation step]') % x-axis label
ylabel('Hausdorff [pixel] ') % y-axis label
title( {'2D Hausdorff Distance'; '2D  Ground Truth Catheter-  2D Filter Catheter' }) ;