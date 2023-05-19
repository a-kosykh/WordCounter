#ifndef CONTENTPROCESSOR_H
#define CONTENTPROCESSOR_H

#include <QColor>
#include <QVariantList>
#include <QMap>
#include <QObject>
#include <QMutex>

class ContentProcessor : public QObject
{
    Q_OBJECT
    struct WordProperties{
        int count;
        QColor color;
    };
public:
    explicit ContentProcessor(QObject *parent = nullptr);
    void process(const QString& line, quint64 processedBytes, quint64 totalBytes);
    QVariantList getTopWords();
    double getProgress();
    int getMaxCount();

    void clearContents();
    void setAbortTrue();
    void setAbortFalse();

public:
    const QString wordProp = "word";
    const QString countProp = "count";
    const QString colorProp = "color";
    const int topCount = 15;

signals:
    void topWordsChanged();
    void progressChanged();

private:
    QMutex m_mutex;
    double m_progress;
    QHash<QString, WordProperties> m_wordsCount;
    QVariantList m_topWords;

private:
    void setTopWords(const QVariantList& topWords);
    void setProgress(double progress);

};

#endif // CONTENTPROCESSOR_H
