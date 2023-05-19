#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "contentprocessor.h"
#include "fileloader.h"

#include <QThread>
#include <QObject>

class Controller : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList topWords READ topWords NOTIFY topWordsChanged)
    Q_PROPERTY(double progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(double maxCount READ maxCount NOTIFY maxCountChanged)

public:
    explicit Controller(int linesPerIteration, QObject *parent = nullptr);
    ~Controller();
    QVariantList topWords() const;
    double progress() const;
    int maxCount() const;

signals:
    void topWordsChanged();
    void progressChanged();
    void maxCountChanged();
    void filepathChanged(QString fileUrl);

private:
    QThread m_fileLoaderThread;
    QThread m_contentProcessorThread;
    ContentProcessor* m_contentProcessor;
    FileLoader* m_fileLoader;

};
Q_DECLARE_METATYPE(Controller*)

#endif // CONTROLLER_H
