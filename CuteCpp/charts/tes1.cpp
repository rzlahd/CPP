main.cpp

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 1. Create a new chart
    QChart *chart = new QChart();
    chart->setTitle("My First Chart");
    // 2. Create a line series to hold data
    QLineSeries *series = new QLineSeries();

    // 3. Add some data points (x, y)
    series->append(0, 1);
    series->append(1, 3);
    series->append(2, 2);
    series->append(3, 5);
    series->append(4, 4);

    // 4. Add the series to the chart
    chart->addSeries(series);

    // 5. Create default axes for the chart
    chart->createDefaultAxes();

    // 6. Access the axes directly after creating them
    QAbstractAxis *xAxis = chart->axes(Qt::Horizontal).first();
    QAbstractAxis *yAxis = chart->axes(Qt::Vertical).first();

    if (xAxis) {
        xAxis->setTitleText("X-Axis"); // Set title for the x-axis
    }
    if (yAxis) {
        yAxis->setTitleText("Y-Axis"); // Set title for the y-axis
    }

    // 7. Create a QChartView to display the chart
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing); // Make the rendering smoother

    // 8. Find the layout of your main window and add the chart view to it
    if (ui->verticalLayout) { // Assuming you have a vertical layout in your UI
            ui->verticalLayout->addWidget(chartView);
    } else {
        // If you don't have a layout, you might need to set the central widget directly
        setCentralWidget(chartView);
        qDebug() << "Warning: No layout found in UI. Setting chart view as central widget.";
    }
}

main.h

#include <QtGui/QPainter>
#include <QtDebug>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QChartView *chartView; // To hold our chart view
};
#endif // MAINWINDOW_H
