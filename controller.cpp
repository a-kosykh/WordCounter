#include "controller.h"

#include "fileloader.h"
#include <QDebug>

Controller::Controller(QObject *parent)
    : QObject{parent}
{
    FileLoader* fileLoader = new FileLoader;
    fileLoader->moveToThread(&m_fileLoaderThread);
    connect(&m_fileLoaderThread, &QThread::finished, fileLoader, &FileLoader::deleteLater);

    m_contentProcessor = new ContentProcessor;
    m_contentProcessor->moveToThread(&m_contentProcessorThread);
    connect(&m_contentProcessorThread, &QThread::finished, m_contentProcessor, &ContentProcessor::deleteLater);

    connect(this, &Controller::buttonClicked, fileLoader, &FileLoader::openFile);
    connect(fileLoader, &FileLoader::partParsed, m_contentProcessor, &ContentProcessor::process);
    connect(m_contentProcessor, &ContentProcessor::topWordsChanged, this, &Controller::topWordsChanged);
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

double Controller::progress()
{
    return m_contentProcessor->getProgress();
}
