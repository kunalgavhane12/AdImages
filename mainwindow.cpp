#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QPixmap>
#include <QStandardPaths>
#include <QSettings>
#include <QDir>
#include <QFileInfoList>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), currentImageIndex(0)
{
    // Set minimum size for the window
    setMinimumSize(800, 400);

    // Create QLabel as the central widget
    displayLabel = new QLabel(this);
    displayLabel->setAlignment(Qt::AlignCenter);
    displayLabel->setScaledContents(true); // Ensure content scales with the window
    setCentralWidget(displayLabel);

    // Initialize the timer
    imageTimer = new QTimer(this);
    connect(imageTimer, &QTimer::timeout, this, &MainWindow::showNextImage);

    // Load image files
    loadImageFiles();

    // Start cycling through images if available
    if (!imagePaths.isEmpty()) {
        imageTimer->start(5000); // Show each image for 5 seconds
        showNextImage();
    }

    // Set the window to full screen
    showFullScreen();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadImageFiles()
{
    // Get the user's home directory dynamically
    QString homeDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);

    // Define the relative directory for images
    QString imageDirPath = homeDir + "/images";

    // Retrieve the saved path from application settings (if it exists)
    QSettings settings;
    imageDirPath = settings.value("media/imageDir", imageDirPath).toString();

    // If path is not saved, ask the user to choose it
    if (imageDirPath.isEmpty()) {
        imageDirPath = QFileDialog::getExistingDirectory(this, "Select Image Directory", QString(),
                                                         QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        settings.setValue("media/imageDir", imageDirPath); // Save the selected path
    }

    QDir imageDir(imageDirPath);

    // Check if the directory exists
    if (!imageDir.exists()) {
        qDebug() << "The directory does not exist!";
        return;
    }

    // Get the list of all image files (.png, .jpg, .jpeg, etc.)
    QStringList imageFilters;
    imageFilters << "*.png" << "*.jpg" << "*.jpeg"; // Add any other image formats you need
    QFileInfoList imageFiles = imageDir.entryInfoList(imageFilters, QDir::Files);

    // Clear previous list (if needed)
    imagePaths.clear();

    // Add image file paths to the imagePaths list
    for (const QFileInfo &fileInfo : imageFiles) {
        imagePaths << fileInfo.absoluteFilePath();
    }
}

void MainWindow::showNextImage()
{
    if (imagePaths.isEmpty())
        return;

    // Show the next image
    QPixmap pixmap(imagePaths[currentImageIndex]);
    displayLabel->setPixmap(pixmap);
    displayLabel->setScaledContents(true);
    displayLabel->show();

    // Increment the image index
    currentImageIndex++;
    if (currentImageIndex >= imagePaths.size()) {
        currentImageIndex = 0; // Loop back to the first image
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    displayLabel->setGeometry(this->rect());
}
