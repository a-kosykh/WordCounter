#ifndef FILELOADER_H
#define FILELOADER_H

#include <QVariantList>
#include <QHash>
#include <QFile>
#include <QObject>

class FileLoader : public QObject
{
    Q_OBJECT
public:
    explicit FileLoader(QObject *parent = nullptr);
    void openFile();
    void processLine(const QString& line);
    QVariantList getTopWords() const;

signals:
    void lineParsed(const QString& fileContent);
    void topWordsChanged();

private:
    QVariantList m_topWords;
    QHash<QString, int> m_wordsCount;
    QByteArray m_content;
    QFile m_file;

private:
    void handler(const QString& filename);
    void setTopWords(const QVariantList& topWords);

};

#endif // FILELOADER_H
