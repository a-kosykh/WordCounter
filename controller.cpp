#include "controller.h"

#include "fileloader.h"
#include <QDebug>

Controller::Controller(QObject *parent)
    : QObject{parent}
{
    m_fileLoader = new FileLoader;
    m_fileLoader->moveToThread(&m_fileLoaderThread);
    connect(this, &Controller::buttonClicked, m_fileLoader, &FileLoader::openFile);
    connect(m_fileLoader, &FileLoader::topWordsChanged, this, &Controller::topWordsChanged);
    connect(&m_fileLoaderThread, &QThread::finished, m_fileLoader, &FileLoader::deleteLater);

    m_fileLoaderThread.start();
}

Controller::~Controller()
{
    m_fileLoaderThread.requestInterruption();

    m_fileLoaderThread.quit();
    m_fileLoaderThread.wait();
}

QVariantList Controller::topWords() const
{
    return m_fileLoader->getTopWords();
}
