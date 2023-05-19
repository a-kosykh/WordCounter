#include "controller.h"

#include "fileloader.h"
#include <QDebug>

Controller::Controller(int linesPerIteration, QObject *parent)
    : QObject{parent}
{
    m_fileLoader = new FileLoader(linesPerIteration);
    m_fileLoader->moveToThread(&m_fileLoaderThread);
    connect(&m_fileLoaderThread, &QThread::finished, m_fileLoader, &FileLoader::deleteLater);

    m_contentProcessor = new ContentProcessor;
    m_contentProcessor->moveToThread(&m_contentProcessorThread);
    connect(&m_contentProcessorThread, &QThread::finished, m_contentProcessor, &ContentProcessor::deleteLater);

    connect(this, &Controller::filepathChanged, m_fileLoader, &FileLoader::openFile);
    connect(this, &Controller::filepathChanged, m_contentProcessor, &ContentProcessor::clearContents);
    connect(m_fileLoader, &FileLoader::partParsed, m_contentProcessor, &ContentProcessor::process);
    connect(m_contentProcessor, &ContentProcessor::topWordsChanged, this, &Controller::topWordsChanged);
    connect(m_contentProcessor, &ContentProcessor::topWordsChanged, this, &Controller::maxCountChanged);
    connect(m_contentProcessor, &ContentProcessor::progressChanged, this, &Controller::progressChanged);

    m_fileLoaderThread.start();
    m_contentProcessorThread.start();
}

Controller::~Controller()
{
    m_contentProcessorThread.requestInterruption();

    m_fileLoaderThread.quit();
    m_fileLoaderThread.wait();

    m_contentProcessorThread.quit();
    m_contentProcessorThread.wait();
}


QVariantList Controller::topWords() const
{
    return m_contentProcessor->getTopWords();
}

double Controller::progress() const
{
    return m_contentProcessor->getProgress();
}

int Controller::maxCount() const
{
    return m_contentProcessor->getMaxCount();
}
