function stop = optimplotfval(~,optimValues,state,varargin)
% OPTIMPLOTFVAL Plot value of the objective function at each iteration.
%
%   STOP = OPTIMPLOTFVAL(X,OPTIMVALUES,STATE) plots OPTIMVALUES.fval.  If
%   the function value is not scalar, a bar plot of the elements at the
%   current iteration is displayed.  If the OPTIMVALUES.fval field does not
%   exist, the OPTIMVALUES.residual field is used.
%
%   Example:
%   Create an options structure that will use OPTIMPLOTFVAL as the plot
%   function
%     options = optimset('PlotFcns',@optimplotfval);
%
%   Pass the options into an optimization problem to view the plot
%     fminbnd(@sin,3,10,options)

%   Copyright 2006-2010 The MathWorks, Inc.
%   $Revision: 1.1.6.2 $  $Date: 2012/08/21 00:30:41 $

stop = false;
switch state
    case 'iter'
        if isfield(optimValues,'fval')
            if isscalar(optimValues.fval)
                plotscalar(optimValues.iteration,optimValues.fval);
            else
                plotvector(optimValues.iteration,optimValues.fval);
            end
        else
            plotvector(optimValues.iteration,optimValues.residual);
        end
end

function plotscalar(iteration,fval)
% PLOTSCALAR initializes or updates a line plot of the function value
% at each iteration.

if iteration == 0
    axes(findobj(gcf,'Tag','Fval_axes'))    
    plotfval = plot(iteration,fval,'b.');    
    title(['CurrentFunctionValue ' sprintf('%g',fval)]);
    xlabel('LabelIteration');
    set(plotfval,'Tag','optimplotfval');
    ylabel('LabelFunctionValue');
else
    all_lines= findobj(gcf,'Type', 'Line');
    line_optimfval= findobj(all_lines,'Tag','optimplotfval');
    newX = [get(line_optimfval,'Xdata') iteration];
    newY = [get(line_optimfval,'Ydata') fval];
    set(line_optimfval,'Xdata',newX, 'Ydata',newY);
    
    % find the correct axes
    all_axes= findobj(gcf,'Type', 'Axes');     
    for i= 1:length(all_axes)
        if ~isempty(regexp(all_axes(i).Title.String,'Current*','once'))
            % change the axis title
            title(all_axes(i),['CurrentFunctionValue ',sprintf('%g',fval)]);
        end
    end
end

function plotvector(iteration,fval)
% PLOTVECTOR creates or updates a bar plot of the function values or
% residuals at the current iteration.
if iteration == 0
    xlabelText = ['NumberOfFunctionValues ',sprintf('%g',length(fval))];
    % display up to the first 100 values
    if numel(fval) > 100
        xlabelText = {xlabelText,['ShowingOnlyFirst100Values']};
        fval = fval(1:100);
    end
    plotfval = bar(fval);
    title(['TitleCurrentFunctionValues']);
    set(plotfval,'edgecolor','none')
    set(gca,'xlim',[0,1 + length(fval)])
    xlabel(xlabelText,'interp','none')
    set(plotfval,'Tag','optimplotfval');
    ylabel(['FunctionValue'])
else
    plotfval = findobj(get(gca,'Children'),'Tag','optimplotfval');
    % display up to the first 100 values
    if numel(fval) > 100
        fval = fval(1:100);
    end
    set(plotfval,'Ydata',fval);
end
