#include "stdafx.h"
#include "HistoryControlPage.h"
#include "HistoryControlPageThemePanel.h"
#include "../../controls/BackButton.h"
#include "../../controls/CommonStyle.h"
#include "../../controls/CustomButton.h"
#include "../../controls/LabelEx.h"
#include "../../utils/InterConnector.h"
#include "../../utils/utils.h"
#include "../../fonts.h"

namespace Ui
{
    HistoryControlPageThemePanel::HistoryControlPageThemePanel(HistoryControlPage* _parent)
        : QWidget(_parent), historyControlPage_(_parent), selectionThemeFromSettings_(false), settingThemeToAll_(false)
    {
        QHBoxLayout *mainLayout = Utils::emptyHLayout(this);
        mainLayout->setContentsMargins(Utils::scale_value(16), 0, Utils::scale_value(16), 0);

        backFromThemeButton_ = new BackButton(this);
        connect(backFromThemeButton_, &BackButton::clicked, this, &HistoryControlPageThemePanel::backFromThemePressed, Qt::QueuedConnection);
        backFromThemeButton_->setCursor(Qt::PointingHandCursor);
        mainLayout->addWidget(backFromThemeButton_);

        mainLayout->addSpacerItem(new QSpacerItem(Utils::scale_value(16), 0, QSizePolicy::Expanding, QSizePolicy::Preferred));

        cancelButton_ = new LabelEx(this);
        cancelButton_->setText(QT_TRANSLATE_NOOP("chat_page", "CANCEL"));
        cancelButton_->setColor(CommonStyle::getColor(CommonStyle::Color::TEXT_SECONDARY));
        cancelButton_->setFont(Fonts::appFontScaled(15, Fonts::FontWeight::Medium));
        cancelButton_->setCursor(Qt::PointingHandCursor);
        cancelButton_->adjustSize();
        mainLayout->addWidget(cancelButton_, 0, Qt::AlignRight);
        connect(cancelButton_, &LabelEx::clicked, this, &HistoryControlPageThemePanel::cancelThemePressed, Qt::QueuedConnection);

        mainLayout->addSpacerItem(new QSpacerItem(Utils::scale_value(32), 0, QSizePolicy::Fixed, QSizePolicy::Fixed));

        setToAllButton_ = new QPushButton(QT_TRANSLATE_NOOP("chat_page", "SET TO ALL"));
        Utils::ApplyStyle(setToAllButton_, CommonStyle::getGreenButtonStyle());
        setToAllButton_->setCursor(Qt::PointingHandCursor);
        mainLayout->addWidget(setToAllButton_, 0, Qt::AlignRight);
        connect(setToAllButton_, &QPushButton::clicked, this, &HistoryControlPageThemePanel::setToAllThemePressed, Qt::QueuedConnection);

        mainLayout->addSpacerItem(new QSpacerItem(Utils::scale_value(16), 0, QSizePolicy::Fixed, QSizePolicy::Fixed));

        setButton_ = new QPushButton(QT_TRANSLATE_NOOP("chat_page", "SET"));
        Utils::ApplyStyle(setButton_, CommonStyle::getGreenButtonStyle());
        setButton_->setCursor(Qt::PointingHandCursor);
        mainLayout->addWidget(setButton_);
        connect(setButton_, &QPushButton::clicked, this, &HistoryControlPageThemePanel::setThemePressed, Qt::QueuedConnection);

        setVisible(false);
        setLayout(mainLayout);
    }

    HistoryControlPageThemePanel::~HistoryControlPageThemePanel()
    {
    }

    void HistoryControlPageThemePanel::setShowSetThemeButton(const bool _show)
    {
        if (_show)
        {
            selectionThemeFromSettings_ = true;
        }
        setButton_->setVisible(_show);
    }

    void HistoryControlPageThemePanel::setCallback(ThemePanelCallback _callback)
    {
        callback_ = _callback;
    }

    void HistoryControlPageThemePanel::cancelThemePressed()
    {
        if (callback_)
        {
            callback_(ThemePanelCancel);
            callback_ = nullptr;
        }
        historyControlPage_->showMainTopPanel();
    }

    void HistoryControlPageThemePanel::setThemePressed()
    {
        if (callback_)
        {
            callback_(ThemePanelSet);
            callback_ = nullptr;
        }
        historyControlPage_->showMainTopPanel();
    }

    void HistoryControlPageThemePanel::setToAllThemePressed()
    {
        if (callback_)
        {
            callback_(ThemePanelSetToAll);
            callback_ = nullptr;
        }
        historyControlPage_->showMainTopPanel();
    }

    void HistoryControlPageThemePanel::backFromThemePressed()
    {
        const QString targetContact = settingThemeToAll_ ? QString() : historyControlPage_->aimId();
        emit Utils::InterConnector::instance().themesSettingsShow(selectionThemeFromSettings_, targetContact);
        selectionThemeFromSettings_ = false; // flush
        if (callback_)
        {
            callback_(ThemePanelBackToSettings);
            callback_ = nullptr;
        }
        historyControlPage_->showMainTopPanel();
    }

    void HistoryControlPageThemePanel::setSelectionToAll(bool _settingThemeToAll)
    {
        settingThemeToAll_ = _settingThemeToAll;
    }

    void HistoryControlPageThemePanel::paintEvent(QPaintEvent *_e)
    {
        QWidget::paintEvent(_e);

        QPainter painter(this);
        painter.fillRect(rect(), CommonStyle::getFrameColor());
        painter.setPen(QPen(CommonStyle::getColor(CommonStyle::Color::GRAY_BORDER), Utils::scale_value(1)));
        painter.drawLine(contentsRect().bottomLeft(), contentsRect().bottomRight());
    }
}
