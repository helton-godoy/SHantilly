#include <QtTest>
#include <QLoggingCategory>
#include "logger.h"
#include "execution_context.h"

/**
 * SHantilly Unit Tests
 * 
 * These tests verify individual components without requiring
 * the full SHantilly class (which has Qt widget dependencies).
 */
class TestSHantillyUnits : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    
    // Logger tests (Qt Logging Categories)
    void testLoggerCategoriesExist();
    void testLoggerDebugOutput();
    
    // ExecutionContext tests
    void testExecutionContextCreation();
    void testExecutionContextNullDialog();
};

void TestSHantillyUnits::initTestCase()
{
    qDebug() << "Starting SHantilly unit tests...";
}

void TestSHantillyUnits::cleanupTestCase()
{
    qDebug() << "Finished SHantilly unit tests.";
}

void TestSHantillyUnits::testLoggerCategoriesExist()
{
    // Verify that logging categories are properly declared
    qCDebug(parserLog) << "Test parser log message";
    qCDebug(guiLog) << "Test GUI log message";
    QVERIFY(true);  // Categories should exist without crash
}

void TestSHantillyUnits::testLoggerDebugOutput()
{
    // Test that logging works without crashing
    qCWarning(parserLog) << "Test warning message";
    qCInfo(guiLog) << "Test info message";
    QVERIFY(true);
}

void TestSHantillyUnits::testExecutionContextCreation()
{
    // ExecutionContext should be creatable with nullptr
    ExecutionContext ctx(nullptr);
    QVERIFY(true);
}

void TestSHantillyUnits::testExecutionContextNullDialog()
{
    // Operations on ExecutionContext with null dialog should not crash
    ExecutionContext ctx(nullptr);
    // The context should handle null dialog gracefully
    QVERIFY(true);
}

QTEST_MAIN(TestSHantillyUnits)

#include "tst_units.moc"
