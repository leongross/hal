//  MIT License
//
//  Copyright (c) 2019 Ruhr University Bochum, Chair for Embedded Security. All Rights reserved.
//  Copyright (c) 2021 Max Planck Institute for Security and Privacy. All Rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.

#pragma once

#include "gui/channel_manager/channel_item.h"
#include "gui/content_widget/content_widget.h"
#include "hal_core/utilities/log.h"
#include "gui/gui_utils/graphics.h"

#include <QLabel>
#include <QMenu>
#include <QPlainTextEdit>
#include <QString>
#include <QToolButton>
#include <QWidget>
#include <QtCore/qreadwritelock.h>
#include <QScrollBar>
#include <QPushButton>

namespace hal
{
    class LoggerMarshall;
    class ChannelSelector;
    class SeveritySelector;

    /**
     * @ingroup logging
     * @brief Displays the logs in the gui.
     *
     * The LoggerWidget is the content widget that displays the current hal log.
     */
    class LoggerWidget : public ContentWidget
    {
        Q_OBJECT

    public:
        /**
         * Constructor.
         *
         * @param parent - The parent widget
         */
        LoggerWidget(QWidget* parent = nullptr);
        ~LoggerWidget() override;

        /**
         * Initializes the toolbar of the LoggerWidget. It creates the ChannelSelector for the logger channels within
         * the toolbar.
         *
         * @param Toolbar - The toolbar to initialize
         */
        virtual void setupToolbar(Toolbar* Toolbar) override;

        /**
         * Gets the underlying (read only) QPlainTextEdit object this LoggerWidget uses to display the log.
         *
         * @returns the underlying QPlainTextEdit object
         */
        QPlainTextEdit* getPlainTextEdit();

        /**
         * Overrides the QWidget resizeEvent. After resizing the logger should be scrolled to the bottom, but only
         * if the user hasn't interacted with the scrollbar before.
         *
         * @param event
         */
        void resizeEvent(QResizeEvent* event) override;

    public Q_SLOTS:
        /**
         * Appends the log message to the appropriate filter.
         *
         * @param t - The enum level.
         * @param msg - The new log message.
         */
        void handleCurrentChannelUpdated(spdlog::level::level_enum t, QString const& msg);

        /**
         * Q_SLOT to handle that the log manager has received a new message.
         *
         * @param t - The type of the message
         * @param logger_name - The channel of the message
         * @param msg - The message itself
         */
        void handleChannelUpdated(spdlog::level::level_enum t, const std::string& logger_name, std::string const& msg);

        /**
         * Q_SLOT to handle that the currently selected filter has been changed (e.g. by choosing another one in the
         * the ChannelSelector combobox or by toggling a severity checkbox).
         *
         * @param p - New channel index or new state of a severity checkbox
         */
        void handleCurrentFilterChanged(int index);

        void handleSeverityChanged(bool state);

        /**
         * Q_SLOT to handle interactions with the scrollbar. After the first scrollbar interaction the scrollbar wont
         * be locked at the bottom anymore (e.g. after a resize event).
         *
         * @param value - The new scroll position of the scrollbar (unused)
         */
        void handleFirstUserInteraction(int value);

    private:
        void scrollToBottom();

        QPlainTextEdit* mPlainTextEdit;
        //ChannelSelector* mSelector;
        ChannelSelector* selector;
        QPushButton* mDebugButton;
        QPushButton* mInfoButton;
        QPushButton* mWarningButton;
        QPushButton* mErrorButton;
//        SeveritySelector* mInfoSelector;
//        SeveritySelector* mWarningSelector;
//        SeveritySelector* mErrorSelector;
//        SeveritySelector* mDebugSelector;
        bool mInfoSeverity;
        bool mWarningSeverity;
        bool mErrorSeverity;
        bool mDebugSeverity;


        LoggerMarshall* mLogMarshall;
        int mCurrentChannelIndex;
        std::string mCurrentChannel;
        QReadWriteLock mLock;
        QScrollBar* mPlainTextEditScrollbar;
        bool mUserInteractedWithScrollbar;
    };
}
