import QtQuick 2.4
import Material 0.1
import Material.ListItems 0.1 as ListItem
import SocialBlock.connector 1.0

ApplicationWindow {
    id: window

    title: "SocialBlock"
    visible: true

    theme {
        primaryColor: Palette.colors["blue"]["500"]
        primaryDarkColor: Palette.colors["blue"]["700"]
        accentColor: Palette.colors["red"]["A200"]
        tabHighlightColor: "white"
    }

    property var days: [
            "Monday", "Tuesday", "Wednesday", "Thursday",
            "Friday", "Saturday", "Sunday"
    ]

    property int selectedProf: manager.getCurrentProfileNumber()

    property int selectedDay: {
        var d = new Date();
        return d.getDay() - 1
    }

    SBConnector {
        id: manager

        onExitApplication: Qt.quit();
        onOpenApplication: window.show();

    property var sectionTitles: manager.getProfileNames()

    initialPage: TabbedPage {
        id: page

        title: "SocialBlock"

        actionBar.maxActionCount: 4

        actions: [
            Action {
                iconName: manager.istracked ? "av/pause_circle_outline" : "av/play_circle_outline"
                name: manager.istracked? "Stop blocking" : "Start blocking!"
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
                name: "Settings"
                hoverAnimation: true
                onTriggered: window.showError(selectedProf.toString(), "Do you want to retry?", "Close", true)
            },


            Action {
                iconName: "action/assignment_turned_in"
                name: "Set as current profile"
                enabled: selectedProf !== manager.getCurrentProfileNumber()
                onTriggered: {
                    manager.setCurrentProfileNumber(selectedProf)
                    enabled = false
                }
            },

            Action {
                iconName: "awesome/user_plus"
                name: "Add profile"
                onTriggered: window.showError("Something went wrong", "Do you want to retry?", "Close", true)
            },

            Action {
                iconName: "awesome/user_times"
                name: "Delete profile"
                onTriggered: colorPicker.show()
            },

            Action {
                iconName: "action/alarm_add"
                name: "Add time interval"
            },


            Action {
                iconName: "image/color_lens"
                name: "Colors"
                onTriggered: colorPicker.show()
            },

            Action {
                iconName: "content/clear"
                name: "Exit"
                hoverAnimation: true
                onTriggered: manager.exitTriggered()
            }
        ]

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
                        anchors.left: parent.left
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
                                                visible: !manager.isblocked
                                                MouseArea {
                                                    anchors.fill: parent
                                                    onClicked:{
                                                        manager.deleteInterval(selectedDay, selectedDay, index)
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
}
