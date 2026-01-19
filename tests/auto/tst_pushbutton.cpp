#include <push_button_widget.h>

#include <QtTest>

class TestPushButton : public QObject {
    Q_OBJECT

private slots:
    void initTestCase();
    void testProperties();
};

void TestPushButton::initTestCase() {
    // Called before the first testfunction is executed
}

void TestPushButton::testProperties() {
    PushButtonWidget btn("Test Label");
    QCOMPARE(btn.text(), QString("Test Label"));
    QVERIFY(btn.isCheckable() == false); // Default
}

QTEST_MAIN(TestPushButton)
#include "tst_pushbutton.moc"
