clear 
close all
clc

simple_ob1=hgload('simple2DHD_OB.fig');
simple_ob2=hgload('simple3DHD_OB.fig');
simple_ob3=hgload('simple2DRMSE_OB.fig');
simple_ob4=hgload('simple3DRMSE_OB.fig');

simple_top1=hgload('simple2DHD_TOP.fig');
simple_top2=hgload('simple3DHD_TOP.fig');
simple_top3=hgload('simple2DRMSE_TOP.fig');
simple_top4=hgload('simple3DRMSE_TOP.fig');

simple_SIDE1=hgload('simple2DHD_SIDE.fig');
simple_SIDE2=hgload('simple3DHD_SIDE.fig');
simple_SIDE3=hgload('simple2DRMSE_SIDE.fig');
simple_SIDE33=hgload('simple3DRMSE_SIDE.fig');
simple_SIDE4=hgload('simple3DRMSE_SIDE.fig');


h(1)=subplot(3,4,1);
grid on
grid minor
xlabel('h - [simulation step]') % x-axis label
ylabel('Haussdorf [pixel] ') % y-axis label
title( {'{\it Simple Vessel Oblique View}';'Haussdorf Distance '  }) 
ax = gca;
ax.TitleFontSizeMultiplier = 1;
h(2)=subplot(3,4,2);
grid on
grid minor
xlabel('h - [simulation step]') % x-axis label
ylabel('Haussdorf [mm] ') % y-axis label
title( {'{\it Simple Vessel Oblique View}';'3D Haussdorf Distance'  }) 
ax = gca;
ax.TitleFontSizeMultiplier = 1;
h(3)=subplot(3,4,3);
xlabel('h - [simulation step]') % x-axis label
ylabel('RMSE [pixel] ') % y-axis label
title( {'{\it Simple Vessel Oblique View}';'RMSE 2D Ground Truth Tip - 2D  Filter Tip'  }) 
ax = gca;
ax.TitleFontSizeMultiplier = 1;
grid on
grid minor
h(4)=subplot(3,4,4);
grid on
grid minor
xlabel('h - [simulation step]') % x-axis label
ylabel('RMSE [mm] ') % y-axis label
title( {'{\it Simple Vessel Oblique View}';'RMSE 3D Ground Truth Tip - 3D  Filter Tip'  }) 
ax = gca;
ax.TitleFontSizeMultiplier = 1;
%%

h(5)=subplot(3,4,5);
grid on
grid minor
xlabel('h - [simulation step]') % x-axis label
ylabel('Haussdorf [pixel] ') % y-axis label
title( {'{\it Simple Vessel Top View}';'Haussdorf Distance '  }) 
ax = gca;
ax.TitleFontSizeMultiplier = 1;
h(6)=subplot(3,4,6);
grid on
grid minor
xlabel('h - [simulation step]') % x-axis label
ylabel('Haussdorf [mm] ') % y-axis label
title( {'{\it Simple Vessel Top View}';'3D Haussdorf Distance'  }) 
ax = gca;
ax.TitleFontSizeMultiplier = 1;
h(7)=subplot(3,4,7);
xlabel('h - [simulation step]') % x-axis label
ylabel('RMSE [pixel] ') % y-axis label
title( {'{\it Simple Vessel Top View}';'RMSE 2D Ground Truth Tip - 2D  Filter Tip'  }) 
ax = gca;
ax.TitleFontSizeMultiplier = 1;
grid on
grid minor
h(8)=subplot(3,4,8);
grid on
grid minor
xlabel('h - [simulation step]') % x-axis label
ylabel('RMSE [mm] ') % y-axis label
title( {'{\it Simple Vessel Top View}';'RMSE 3D Ground Truth Tip - 3D  Filter Tip'  }) 
ax = gca;
ax.TitleFontSizeMultiplier = 1;

%%

h(9)=subplot(3,4,9);
grid on
grid minor
xlabel('h - [simulation step]') % x-axis label
ylabel('Haussdorf [pixel] ') % y-axis label
title( {'{\it Simple Vessel Side View}';' Haussdorf Distance '  }) 
ax = gca;
ax.TitleFontSizeMultiplier = 1;
h(10)=subplot(3,4,10);
grid on
grid minor
xlabel('h - [simulation step]') % x-axis label
ylabel('Haussdorf [mm] ') % y-axis label
title( {'{\it Simple Vessel Side View}';'3D Haussdorf Distance'  }) 
ax = gca;
ax.TitleFontSizeMultiplier = 1;

h(11)=subplot(3,4,11);
xlabel('h - [simulation step]') % x-axis label
ylabel('RMSE [pixel] ') % y-axis label
title( {'{\it Simple Vessel Side View}';'RMSE 2D Ground Truth Tip - 2D  Filter Tip'  }) 
ax = gca;
ax.TitleFontSizeMultiplier = 1;
grid on
grid minor

h(12)=subplot(3,4,12);
xlabel('h - [simulation step]') % x-axis label
ylabel('RMSE [mm] ') % y-axis label
title( {'{\it Simple Vessel Side View}';'RMSE 3D Ground Truth Tip - 3D  Filter Tip'  }) 
ax = gca;
ax.TitleFontSizeMultiplier = 1;
grid on
grid minor

% Paste figures on the subplots
copyobj(allchild(get(simple_ob1,'CurrentAxes')),h(1));
copyobj(allchild(get(simple_ob2,'CurrentAxes')),h(2));
copyobj(allchild(get(simple_ob3,'CurrentAxes')),h(3));
copyobj(allchild(get(simple_ob4,'CurrentAxes')),h(4));

copyobj(allchild(get(simple_top1,'CurrentAxes')),h(5));
copyobj(allchild(get(simple_top2,'CurrentAxes')),h(6));
copyobj(allchild(get(simple_top3,'CurrentAxes')),h(7));
copyobj(allchild(get(simple_top4,'CurrentAxes')),h(8));

copyobj(allchild(get(simple_SIDE1,'CurrentAxes')),h(9));
copyobj(allchild(get(simple_SIDE2,'CurrentAxes')),h(10));
copyobj(allchild(get(simple_SIDE3,'CurrentAxes')),h(11));
copyobj(allchild(get(simple_SIDE33,'CurrentAxes')),h(12));


% % Add legends
% l(1)=legend(h(1),'LegendForFirstFigure')
% l(2)=legend(h(2),'LegendForSecondFigure')
