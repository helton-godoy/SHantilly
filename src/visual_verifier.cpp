#include <QApplication>
#include <QCheckBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

#include "SHantillyBuilder.h"
#include "WidgetConfigs.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    SHantillyBuilder builder;

    // 1. Main Window
    Sbx::Models::WindowConfig winConfig;
    winConfig.title = "SHantilly Suite - Full Widget Gallery (V1 Parity)";
    winConfig.width = 800;
    winConfig.height = 900;
    QWidget* mainWindow = builder.buildWindow(winConfig);
    QVBoxLayout* mainLayout = qobject_cast<QVBoxLayout*>(mainWindow->layout());

    // Header
    QLabel* header = new QLabel("<h1>SHantilly Suite - Parity Demo</h1>");
    header->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(header);

    // Scroll Area for content
    QScrollArea* scroll = new QScrollArea();
    QWidget* content = new QWidget();
    QVBoxLayout* contentLayout = new QVBoxLayout(content);
    scroll->setWidget(content);
    scroll->setWidgetResizable(true);
    mainLayout->addWidget(scroll);

    // --- Tab Widget as Top Level Organizer ---
    Sbx::Models::TabWidgetConfig mainTabs;
    Sbx::Models::PageConfig pBasics, pInputs, pContainers, pAdvanced;

    pBasics.title = "Basic Controls";
    pBasics.layout.type = Sbx::Models::SbxLayoutConfig::VBox;

    pInputs.title = "Complex Inputs";
    pInputs.layout.type = Sbx::Models::SbxLayoutConfig::VBox;

    pContainers.title = "Layout & Containers";
    pContainers.layout.type = Sbx::Models::SbxLayoutConfig::VBox;

    pAdvanced.title = "Advanced & Charts";
    pAdvanced.layout.type = Sbx::Models::SbxLayoutConfig::VBox;

    mainTabs.pages << pBasics << pInputs << pContainers << pAdvanced;
    QWidget* tabWidget = builder.buildTabWidget(mainTabs);
    QTabWidget* realTabs = qobject_cast<QTabWidget*>(tabWidget);
    contentLayout->addWidget(tabWidget);

    // --- TAB 1: BASIC CONTROLS ---
    QWidget* t1 = realTabs->widget(0);
    if (t1->layout()) {
        Sbx::Models::LabelConfig lbl;
        lbl.name = "lbl1";
        lbl.text = "Standard Text Label";
        t1->layout()->addWidget(builder.buildLabel(lbl));

        Sbx::Models::ButtonConfig btn;
        btn.text = "Push Button";
        t1->layout()->addWidget(builder.buildButton(btn));

        Sbx::Models::CheckBoxConfig cb;
        cb.name = "cb1";
        cb.text = "CheckBox Option";
        cb.checked = true;
        t1->layout()->addWidget(builder.buildCheckBox(cb));

        QHBoxLayout* radioLay = new QHBoxLayout();
        Sbx::Models::RadioButtonConfig rb1;
        rb1.name = "rb1";
        rb1.text = "Radio 1";
        rb1.checked = true;
        radioLay->addWidget(builder.buildRadioButton(rb1));

        Sbx::Models::RadioButtonConfig rb2;
        rb2.name = "rb2";
        rb2.text = "Radio 2";
        radioLay->addWidget(builder.buildRadioButton(rb2));
        static_cast<QVBoxLayout*>(t1->layout())->addLayout(radioLay);

        Sbx::Models::SeparatorConfig sep;
        sep.name = "sep1";
        sep.orientation = 1; // Horizontal
        t1->layout()->addWidget(builder.buildSeparator(sep));

        Sbx::Models::ComboBoxConfig combo;
        combo.name = "combo1";
        combo.items << "Choice 1"
                    << "Choice 2"
                    << "Choice 3";
        combo.currentIndex = 0;
        t1->layout()->addWidget(builder.buildComboBox(combo));
    }

    // --- TAB 2: COMPLEX INPUTS ---
    QWidget* t2 = realTabs->widget(1);
    if (t2->layout()) {
        t2->layout()->addWidget(new QLabel("LineEdit & TextEdit:"));
        Sbx::Models::LineEditConfig le;
        le.name = "le1";
        le.text = "Value";
        le.placeholder = "Placeholder...";
        t2->layout()->addWidget(builder.buildLineEdit(le));

        Sbx::Models::TextEditConfig te;
        te.name = "te1";
        te.text = "Multi-line <b>Rich Text</b> Editor";
        te.richText = true;
        t2->layout()->addWidget(builder.buildTextEdit(te));

        t2->layout()->addWidget(new QLabel("Numerical (SpinBox & Slider):"));
        Sbx::Models::SpinBoxConfig sb;
        sb.name = "spin1";
        sb.value = 42;
        sb.min = 0;
        sb.max = 100;
        sb.step = 1;
        sb.prefix = "$ ";
        t2->layout()->addWidget(builder.buildSpinBox(sb));

        Sbx::Models::SliderConfig sl;
        sl.name = "sl1";
        sl.value = 75;
        sl.min = 0;
        sl.max = 100;
        sl.orientation = 1; // Horizontal
        t2->layout()->addWidget(builder.buildSlider(sl));

        t2->layout()->addWidget(new QLabel("Calendar:"));
        Sbx::Models::CalendarConfig cal;
        cal.name = "cal1";
        t2->layout()->addWidget(builder.buildCalendar(cal));
    }

    // --- TAB 3: LAYOUT & CONTAINERS ---
    QWidget* t3 = realTabs->widget(2);
    if (t3->layout()) {
        // GroupBox
        Sbx::Models::GroupBoxConfig gb;
        gb.title = "Group Box with VBox Layout";
        gb.layout.type = Sbx::Models::SbxLayoutConfig::VBox;
        QWidget* groupBox = builder.buildGroupBox(gb);
        groupBox->layout()->addWidget(new QPushButton("Inside Group"));
        groupBox->layout()->addWidget(new QCheckBox("Also inside"));
        t3->layout()->addWidget(groupBox);

        // Frame
        Sbx::Models::FrameConfig fr;
        fr.layout.type = Sbx::Models::SbxLayoutConfig::HBox;
        QWidget* frame = builder.buildFrame(fr);
        frame->layout()->addWidget(new QLabel("Inside Frame:"));
        frame->layout()->addWidget(new QLineEdit("Value A"));
        frame->layout()->addWidget(new QLineEdit("Value B"));
        t3->layout()->addWidget(frame);
    }

    // --- TAB 4: ADVANCED ---
    QWidget* t4 = realTabs->widget(3);
    if (t4->layout()) {
        t4->layout()->addWidget(new QLabel("Progress & Lists:"));
        Sbx::Models::ProgressBarConfig pb;
        pb.name = "pb1";
        pb.value = 65;
        pb.format = "Loading %p%";
        t4->layout()->addWidget(builder.buildProgressBar(pb));

        Sbx::Models::ListConfig list;
        list.name = "list1";
        list.items << "Alpha"
                   << "Beta"
                   << "Gamma";
        list.multipleSelection = true;
        t4->layout()->addWidget(builder.buildList(list));

        t4->layout()->addWidget(new QLabel("Data Chart (Phase 1):"));
        Sbx::Models::ChartConfig chart;
        chart.title = "Sales 2026";
        chart.data["Q1"] = 120.5;
        chart.data["Q2"] = 240.0;
        chart.data["Q3"] = 180.2;
        t4->layout()->addWidget(builder.buildChart(chart));
    }

    // Show
    mainWindow->show();

    return app.exec();
}