import QtQuick 2.4
import QtMultimedia 5.5
import Material 0.1
import Material.ListItems 0.1 as ListItem
import SocialBlock.manager 1.0

ApplicationWindow {
    id: window

    title: "SocialBlock"
    visible: true

    theme {
        primaryColor: Palette.colors["blue"]["500"]
        primaryDarkColor: Palette.colors["blue"]["700"]
        accentColor: Palette.colors["blue"]["600"]
        tabHighlightColor: "white"
    }

    property var days: [
            "Monday", "Tuesday", "Wednesday", "Thursday",
            "Friday", "Saturday", "Sunday"
    ]

    SBManager {
        id: manager

        onExitApplication: Qt.quit()
        onOpenApplication: window.show()
    }

    property int selectedProf: manager.getCurrentProfileNumber()

    property int selectedDay: {
        var d = new Date();
        return d.getDay() - 1
    }

    property var sectionTitles: manager.getProfileNames()

    initialPage: TabbedPage {
        id: page

        title: "SocialBlock"

        actionBar.maxActionCount: 4

        actions: [
            Action {
                iconName: manager.istracked ?
                              "av/pause_circle_outline" :
                              "av/play_circle_outline"
                name: manager.istracked ?
                          "Stop blocking" :
                          "Start blocking!"
                onTriggered: manager.trackTriggered();
                enabled: !manager.isblocked
            },

            Action {
                iconName: "content/save"
                name: "Save"
                onTriggered: manager.save()
            },

            Action {
                iconName: "action/settings"
                name: "More"
                hoverAnimation: true
                enabled: !manager.istracked
                onTriggered: actionSheet.open()
            },


            Action {
                iconName: "action/assignment_turned_in"
                name: "Set as current profile"
                enabled: selectedProf !== manager.getCurrentProfileNumber() && !manager.isblocked
                onTriggered: {
                    manager.setCurrentProfileNumber(selectedProf)
                    enabled = false
                }
            },

            Action {
                iconName: "awesome/user_plus"
                name: "Add profile"
                enabled: !manager.istracked
                onTriggered: {
                    addProfDialog.show();
                    //Qt.quit()
                }
            },

            Action {
                iconName: "awesome/user_times"
                name: "Delete profile"
                enabled: !manager.istracked
                onTriggered: {
                    manager.deleteProfile( selectedProf );
                    //Qt.quit()
                }
            },

            Action {
                iconName: "action/alarm_add"
                name: "Add time interval"
                enabled: !manager.istracked
                onTriggered: addIntervalDialog.show()
            },

            Action {
                iconName: "awesome/power_off"
                name: "Exit"
                hoverAnimation: true
                enabled: !manager.istracked
                onTriggered: manager.exitTriggered()
            }
        ]

        BottomActionSheet {
            id: actionSheet

            title: "More"

            actions: [
                Action {
                    iconName: "awesome/vk"
                    name: "Add VK integration"
                },

                Action {
                    iconName: "hardware/memory"
                    name: "Add to autostart"
                    hasDividerAfter: true
                },

                Action {
                    iconName: "awesome/angellist"
                    name: "About"
                    onTriggered: about.show()
                }
            ]
        }

        onSelectedTabChanged: selectedProf = page.selectedTab

        Repeater {
            id: tabRep
            model: sectionTitles
            Tab {
                title: sectionTitles[index]
                Row {
                    anchors.fill: parent
                    Rectangle {
                        id: daysList
                        // left bar with weekdays
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        //anchors.left: parent.left
                        width: parent.width / 4
                        color: "#F5F5F5"
                        Flickable {
                            anchors.fill: parent
                            contentHeight: Math.max(daysView.implicitHeight, height)

                            View {
                                anchors.fill: parent
                                anchors.rightMargin: 30
                                elevation:  4
                                Column {
                                    id: daysView
                                    anchors.fill: parent

                                    Repeater {
                                        model: days
                                        ListItem.Standard {
                                            id: dayListItem
                                            text: index == selectedDay ? "<b>"+days[index]+"<\b>" : days[index]
                                            onClicked: {
                                                selectedDay = index
                                                columnRep.model = manager.getTimesList(selectedProf, selectedDay)
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    Rectangle {
                        id: intervalField
                        // right field with intervals
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.left: daysList.right
                        color: "#F5F5F5"

                        Flickable {
                            id: interFl
                            anchors.fill: parent
                            contentHeight:intervColumn.implicitHeight+60
                            View {
                                anchors.fill: parent
                                anchors.margins: 30
                                anchors.leftMargin: 10
                                elevation: 5
                                Column {
                                    id: intervColumn
                                    anchors.fill: parent

                                    Repeater {
                                        id: columnRep
                                        model: manager.getTimesList(selectedProf, selectedDay)

                                        ListItem.Subtitled {

                                            text: modelData
                                            subText: manager.getSitesList(selectedProf, selectedDay)[index]
                                            maximumLineCount: 3
                                            secondaryItem: Icon {
                                                name: "action/alarm_off"
                                                anchors.verticalCenter: parent.verticalCenter
                                                opacity: manager.istracked ? 0.5 : 1
                                                MouseArea {
                                                    anchors.fill: parent
                                                    enabled: !manager.istracked
                                                    onClicked:{
                                                        manager.deleteInterval(selectedProf, selectedDay, index)
                                                        columnRep.model = manager.getTimesList(selectedProf, selectedDay)
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        Scrollbar {
                            flickableItem: interFl
                        }
                    }
                }
            }
        }
    }

    Dialog {
        id: addIntervalDialog
        title: "Adding interval"
        hasActions: true
        width: 300
        height: 300
        Row {
            id: addIntervDialRow
            anchors.fill: parent
            height: parent.height / 5
            Button {
                id: timeFromButton
                //anchors.left: parent.left
                //anchors.leftMargin: 30
                elevation: 1
                backgroundColor: "#F5F5F5"
                text: "08:00"
                focus: false
                property int hours: 8
                property int minutes: 0
                onClicked: timePickerFrom.show()
                TimePickerDialog {
                    id: timePickerFrom
                    onTimePicked: {
                        timeFromButton.hours = timePicked.getHours()
                        timeFromButton.minutes = timePicked.getMinutes()
                        timeFromButton.text = timePicked.toTimeString().toString().substring(0,5)
                    }
                    prefer24Hour: false
                }
            }
            Label {
                id: lineLabel
                //anchors.left: timeFromButton.right
                //anchors.right: timeToButton.left
                anchors.verticalCenter: timeFromButton.verticalCenter
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                text: "<b>-<\b>"
            }
            Button {
                id: timeToButton
                //anchors.right: parent.right
                anchors.rightMargin: 30
                elevation: 1
                backgroundColor: "#F5F5F5"
                text: "12:00"
                property int hours: 12
                property int minutes: 0
                onClicked: timePickerTo.show()
                TimePickerDialog {
                    id: timePickerTo
                    onTimePicked: {
                        timeToButton.hours = timePicked.getHours()
                        timeToButton.minutes = timePicked.getMinutes()
                        timeToButton.text = timePicked.toTimeString().toString().substring(0,5)
                    }
                    prefer24Hour: false

                }
            }
            Label {
                id: sitesLabel
                x: 0
                y: 50
                width: 50
                height: 5
                text: "<b>Sites:<\b>"
                //anchors.fill: parent
            }
            Rectangle {
                id: sitesEditRect
                x: 0
                y: 70
                width: parent.width
                height: 150
                visible: true
                Flickable {
                    id: flAddInterv
                    anchors.fill: parent
                    contentWidth: textField.width
                    contentHeight: textField.height + 30
                    clip: true
                    TextEdit {
                        id: textField
                        wrapMode: TextEdit.Wrap
                        width: sitesEditRect.width
                        text: "vk.com\n" +
                              "facebook.com\n" +
                              "twitter.com\n" +
                              "instagram.com\n" +
                              "instagr.am\n" +
                              "youtube.com\n" +
                              "ok.ru\n" +
                              "surfinbird.ru\n" +
                              "4pda.ru\n" +
                              "onliner.by\n" +
                              "tut.by\n" +
                              "opennet.ru\n" +
                              "postnauka.ru\n" +
                              "zoom.cnews.ru"
                    }
                }
                Scrollbar {
                    flickableItem: flAddInterv
                }
            }
        }

        positiveButtonText: "OK"
        positiveButtonEnabled: ! ( timeFromButton.hours > timeToButton.hours ||
                                 ( timeFromButton.hours === timeToButton.hours
                                   && timeFromButton.minutes > timeToButton.minutes ) )
        negativeButtonText: "Cancel"
        onAccepted: {
            manager.addInterval ( selectedProf, selectedDay,
                                  timeFromButton.hours, timeFromButton.minutes,
                                  timeToButton.hours, timeToButton.minutes,
                                  textField.text )
            columnRep.model = manager.getTimesList(selectedProf, selectedDay)
            selectedDay += 1
        }
    }
    Dialog {
       id: addProfDialog
       title: "Add new profile"
       width: 256
       height: 330
       hasActions: true
       TextField {
           id: newProfName
           width: parent.width
           placeholderText: "Enter profile name"
       }
       Rectangle {
           id: sitesProfRect
           x: 0
           y: 70
           width: parent.width
           height: 150
           visible: true
           Flickable {
               id: flAddProf
               anchors.fill: parent
               contentWidth: profTextField.width
               contentHeight: profTextField.height
               clip: true
               TextEdit {
                   id: profTextField
                   wrapMode: TextEdit.Wrap
                   width: sitesProfRect.width
                   text: "vk.com\n" +
                         "facebook.com\n" +
                         "twitter.com\n" +
                         "instagram.com\n" +
                         "instagr.am\n" +
                         "youtube.com\n" +
                         "ok.ru\n" +
                         "surfinbird.ru\n" +
                         "4pda.ru\n" +
                         "onliner.by\n" +
                         "tut.by\n" +
                         "opennet.ru\n" +
                         "postnauka.ru\n" +
                         "zoom.cnews.ru"
               }
           }
           Scrollbar {
               flickableItem: flAddProf
           }
       }

       onAccepted: {
           manager.addProfile (newProfName.text, profTextField.text)
           //Qt.quit();
       }
    }

    Dialog {
        id: about
        hasActions: false
        Rectangle {
            height: 13
            width: 30
        }
        Button {
            anchors.topMargin: 20
            x: 36
            elevation: 0
            width: logo.width
            height: logo.height
            Image {
                id: logo
                source: "qrc:/logo.svg"
                sourceSize.width: 150
                sourceSize.height: 150
            }
            onClicked: supermegakeygenmusic.play()

        }
        Text {
            text: "SocialBlock"
            font.pixelSize: 25
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
        }
        onClosed: supermegakeygenmusic.stop()
        Audio {
            id: supermegakeygenmusic
            source: "qrc:/TSRh - Solid Converter PDF v3.0kg.mp3"
        }
        Text {
            textFormat: Text.RichText
            text: "This software is licensed <br>" +
                  "under <a href=\"https://github.com/alex-pat/SocialBlock/blob/master/COPYING\">GNU GPL</a> version 3.<br>" +
                  "Source code is available on <a href='https://github.com/alex-pat/SocialBlock'>GitHub<\a>."
            font.pixelSize: 13
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            onLinkActivated: Qt.openUrlExternally(link)
        }
        Row {
            x: 35
            spacing: 20
            Icon {
                name: "awesome/windows"
                size: 35
            }
            Icon {
                name: "awesome/linux"
                size: 40
            }
            Icon {
                name: "awesome/apple"
                size: 35
            }
        }
        Text {
            text: "Alexander Pateenok, 2015"
            font.pixelSize: 17
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
        }
        Row {
            x: 14
            spacing: 7
            IconButton {
                iconName: "awesome/vk"
                onClicked: Qt.openUrlExternally("http:/vk.com/postskript")
                size: 27
            }
            IconButton {
                iconName: "awesome/facebook_square"
                onClicked: Qt.openUrlExternally("https://www.facebook.com/alexander.pateenok")
                size: 27
            }
            IconButton {
                iconName: "awesome/linkedin_square"
                onClicked: Qt.openUrlExternally("https://www.linkedin.com/in/alexander-pateenok-736302b6")
                size: 27
            }
            IconButton {
                iconName: "awesome/github"
                onClicked: Qt.openUrlExternally("https:/github.com/alex-pat")
                size: 27
            }
            IconButton {
                iconName: "awesome/send"
                onClicked: Qt.openUrlExternally("https:/telegram.me/postskript")
                size: 27
            }
            IconButton {
                iconName: "awesome/twitter"
                onClicked: Qt.openUrlExternally("https://twitter.com/post_skript")
                size: 27
            }
        }
    }
}
