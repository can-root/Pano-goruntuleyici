#include <QApplication>
#include <QClipboard>
#include <QListWidget>
#include <QTabWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>
#include <QTimer>
#include <QStringList>

class PanoUygulamasi : public QWidget {
public:
    PanoUygulamasi(QWidget *parent = nullptr) : QWidget(parent) {
        son_içerik = "";
        geçmiş = QStringList();

        arayüzüKur();

        zamanlayıcı = new QTimer(this);
        connect(zamanlayıcı, &QTimer::timeout, this, &PanoUygulamasi::panoKontrol);
        zamanlayıcı->start(1000);
    }

private:
    QString son_içerik;
    QStringList geçmiş;
    QTextEdit *metinKutusu;
    QListWidget *geçmişListesi;
    QTimer *zamanlayıcı;

    void arayüzüKur() {
        setWindowTitle("Pano İzleyici");
        resize(600, 400);

        QTabWidget *sekme = new QTabWidget(this);

        QWidget *anaSekme = new QWidget(this);
        QVBoxLayout *anaDüzen = new QVBoxLayout(anaSekme);
        metinKutusu = new QTextEdit(this);
        metinKutusu->setReadOnly(true);
        anaDüzen->addWidget(metinKutusu);
        sekme->addTab(anaSekme, "Pano");

        QWidget *geçmişSekmesi = new QWidget(this);
        QVBoxLayout *geçmişDüzen = new QVBoxLayout(geçmişSekmesi);
        geçmişListesi = new QListWidget(this);
        geçmişDüzen->addWidget(geçmişListesi);
        sekme->addTab(geçmişSekmesi, "Geçmiş");

        connect(geçmişListesi, &QListWidget::itemClicked, this, &PanoUygulamasi::geçmiştenKopyala);

        QVBoxLayout *düzen = new QVBoxLayout(this);
        düzen->addWidget(sekme);
        setLayout(düzen);
    }

    void panoKontrol() {
        QClipboard *panoya = QApplication::clipboard();
        QString mevcut_içerik = panoya->text();

        if (mevcut_içerik != son_içerik) {
            son_içerik = mevcut_içerik;
            metniGöster(mevcut_içerik);
            geçmişiGüncelle(mevcut_içerik);
        }
    }

    void metniGöster(const QString &metin) {
        metinKutusu->setPlainText(metin);
    }

    void geçmişiGüncelle(const QString &metin) {
        if (!geçmiş.contains(metin)) {
            geçmiş.append(metin);
            if (geçmiş.size() > 5) {
                geçmiş.removeFirst();
            }
            geçmişListesi->clear();
            geçmişListesi->addItems(geçmiş);
        }
    }

    void geçmiştenKopyala(QListWidgetItem *öğe) {
        QClipboard *panoya = QApplication::clipboard();
        panoya->setText(öğe->text());
    }
};

int main(int argc, char *argv[]) {
    QApplication uygulama(argc, argv);
    PanoUygulamasi panoUygulamasi;
    panoUygulamasi.show();
    return uygulama.exec();
}
