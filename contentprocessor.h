#ifndef CONTENTPROCESSOR_H
#define CONTENTPROCESSOR_H

#include <QVariantList>
#include <QMap>
#include <QObject>

class ContentProcessor : public QObject
{
    Q_OBJECT
public:
    explicit ContentProcessor(QObject *parent = nullptr);
    void process(const QByteArray& content);
    QVariantList getTopWords() const;

signals:
    void topWordsChanged();

private:
    QVariantList m_topWords;

private:
    void setTopWords(const QVariantList& topWords);
};

#endif // CONTENTPROCESSOR_H
