#include "controller.h"

#include "fileloader.h"
#include <QDebug>

Controller::Controller(QObject *parent)
    : QObject{parent}
{
    FileLoader* fileChecker = new FileLoader;
    fileChecker->moveToThread(&m_fileCheckerThread);
    connect(&m_fileCheckerThread, &QThread::finished, fileChecker, &FileLoader::deleteLater);

    m_contentProcessor = new ContentProcessor;
    m_contentProcessor->moveToThread(&m_contentProcessorThread);
    connect(fileChecker, &FileLoader::contentChanged, m_contentProcessor, &ContentProcessor::process);
    connect(m_contentProcessor, &ContentProcessor::topWordsChanged, this, &Controller::debugPrint);
    connect(m_contentProcessor, &ContentProcessor::topWordsChanged, this, &Controller::topWordsChanged);
    connect(&m_contentProcessorThread, &QThread::finished, m_contentProcessor, &ContentProcessor::deleteLater);

    m_fileCheckerThread.start();
    m_contentProcessorThread.start();
}

Controller::~Controller()
{
    m_fileCheckerThread.quit();
    m_fileCheckerThread.wait();

    m_contentProcessorThread.quit();
    m_contentProcessorThread.wait();
}

QVariantList Controller::topWords() const
{
    return m_contentProcessor->getTopWords();
}

void Controller::debugPrint()
{
    //qDebug() << m_contentProcessor->getTopWords();
}
