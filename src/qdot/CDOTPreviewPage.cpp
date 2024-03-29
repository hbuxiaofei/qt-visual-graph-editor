#include "CDOTPreviewPage.h"
#include "ui_CDOTPreviewPage.h"

#include <QFile>
#include <QTemporaryFile>
#include <QDir>
#include <QTextStream>
#include <QProgressDialog>
#include <QProcess>


CDOTPreviewPage::CDOTPreviewPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CDOTPreviewPage)
{
    ui->setupUi(this);

	ui->GraphPreview->setScene(&m_previewScene);

	connect(this, SIGNAL(documentChanged(bool)), parent, SLOT(onDocumentChangedState(bool)));

    ui->LayoutButton->addAction("fdp");
	ui->LayoutButton->addAction("dot");
	ui->LayoutButton->addAction("neato");
	ui->LayoutButton->addAction("sfdp");
	ui->LayoutButton->addAction("circo");
}


CDOTPreviewPage::~CDOTPreviewPage()
{
    delete ui;
}


QString CDOTPreviewPage::errorNotWritable(const QString &path) const
{
	return QObject::tr("Cannot create GraphViz output in %1. Check if the directory is writable.").arg(path);
}


QString CDOTPreviewPage::errorCannotRun(const QString &path) const
{
	return QObject::tr("Cannot run %1. Check if GraphViz has been correctly installed.").arg(path);
}


QString CDOTPreviewPage::errorCannotFinish(const QString &path) const
{
	return QObject::tr("Execution of %1 took too long and has been therefore cancelled by user.").arg(path);
}


bool CDOTPreviewPage::runPreview(const QString &engine, const QString &dotFilePath, QString &svgFilePath, QString* lastError) const
{
	// run dot to convert filename.dot -> filename.temp.svg	
	QTemporaryFile tempFile(QDir::tempPath() + "/qvge-XXXXXX.svg");
	if (!tempFile.open())
	{
		if (lastError)
			*lastError = errorNotWritable(QDir::tempPath());

		return false;
	}

	svgFilePath = tempFile.fileName();
	tempFile.setAutoRemove(false);

	QString pathToDot = "dot";
	//if (m_pathToGraphviz.size())
	//	pathToDot = m_pathToGraphviz + "/dot";

	QString cmd = QString("\"%1\" -K\"%2\" -Tsvg \"%3\" -o\"%4\"").arg(pathToDot, engine).arg(dotFilePath).arg(svgFilePath);

	QProgressDialog progressDialog(tr("Running dot takes longer than expected.\n\nAbort execution?"), tr("Abort"), 0, 100);
	progressDialog.setWindowModality(Qt::ApplicationModal);
	progressDialog.setAutoReset(false);
	progressDialog.setMinimumDuration(1000);

	QProcess process;
	//process.setWorkingDirectory(m_pathToGraphviz);
	process.start(cmd);
	int res = 0;
	process.waitForStarted(1000);
	while (process.state() != QProcess::NotRunning)
	{
		process.waitForFinished(100);
		qApp->processEvents();

		if (progressDialog.wasCanceled())
		{
			if (lastError)
				*lastError = errorCannotFinish(pathToDot);

			return false;
		}

		if (progressDialog.isVisible()) {
			progressDialog.setValue(progressDialog.value() + 1);
			if (progressDialog.value() > 30)
				progressDialog.setMaximum(progressDialog.maximum() + 1);
		}
	}

	//int res = QProcess::execute(cmd);
	if (res != 0)
	{
		if (lastError)
			*lastError = errorCannotRun(pathToDot);

		return false;
	}

	// run successful
	return true;
}


bool CDOTPreviewPage::load(const QString &fileName, QString *lastError)
{
	// fake test
	QFile f(fileName);
	if (!f.open(QFile::ReadOnly))
	{
		if (lastError) *lastError = f.errorString();
		return false;
	}

	m_dotFileName = fileName;

	QTextStream ts(&f);
	ui->DotEditor->setPlainText(ts.readAll());
	f.close();

    return true;
}


void CDOTPreviewPage::on_DotEditor_undoAvailable(bool available)
{
	Q_EMIT documentChanged(available);
}


void CDOTPreviewPage::on_LayoutButton_activated(QVariant data)
{
	m_previewScene.clear();

	QString engine = data.toString();
	QString dotFileName = m_dotFileName;

	bool isChanged = ui->DotEditor->document()->isUndoAvailable();
	if (isChanged)
	{
		QTemporaryFile tempFile(QDir::tempPath() + "/qdot-XXXXXX.dot");
		tempFile.open();
		dotFileName = tempFile.fileName();
		tempFile.setAutoRemove(false);

		QString text = ui->DotEditor->toPlainText();
		tempFile.write(text.toUtf8());
	}

	QString lastError;
	QString svgFileName;
	if (runPreview(engine, dotFileName, svgFileName, &lastError))
	{
		QGraphicsSvgItem *svgItem = new QGraphicsSvgItem(svgFileName);
		m_previewScene.addItem(svgItem);
		m_previewScene.setSceneRect(m_previewScene.itemsBoundingRect());

		ui->GraphPreview->fitInView(svgItem, Qt::KeepAspectRatio);
	}
	else
	{
		//
	}

	QFile::remove(svgFileName);
	if (isChanged)
		QFile::remove(dotFileName);
}
