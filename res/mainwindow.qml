import QtQuick 2.4
import Material 0.1
import Material.ListItems 0.1 as ListItem
import SocialBlock.connector 1.0

ApplicationWindow {
    id: window

    title: "SocialBlock"

    // Necessary when loading the window from C++
    visible: true

    theme {
        primaryColor: Palette.colors["blue"]["500"]
        primaryDarkColor: Palette.colors["blue"]["700"]
        accentColor: Palette.colors["red"]["A200"]
        tabHighlightColor: "white"
    }
    // TODO: ADD PROFILES LOADING
    property var days: [
            "Monday", "Tuesday", "Wednesday", "Thursday",
            "Friday", "Saturday", "Sunday"
    ]

    property int selectedProf: manager.getCurrentProfileNumber()

    property int selectedDay: {
        var d = new Date();
        return d.getDay() - 1
    }

    property var basicComponents: [
            "Button", "CheckBox", "Progress Bar", "Radio Button",
            "Slider", "Switch", "TextField"
    ]

    property var compoundComponents: [
            "Bottom Sheet", "Dialog", "Forms", "List Items", "Page Stack", "Time Picker", "Date Picker"
    ]

//    Repeater {
//        id: weekdays
//        model: manager.getProfilesCount()
//        delegate: days
//    }

//    property var sections: [ days, days, days, days, days, days, days, days, days]

    SBConnector {
        id: manager

        onExitApplication: Qt.quit();
        onOpenApplication: window.show();

//        onSettingsLoaded: {
//            var count = getProfilesCount()
//            for ( var i = 1; i < count; i++)
//                sections.push( days )
//        }
    }

    property var sectionTitles: manager.getProfileNames()

    property string selectedComponent: styles[ manager.getCurrentProfileNumber() ]

    initialPage: TabbedPage {
        id: page

        title: "SocialBlock"

        actionBar.maxActionCount: 4 //navDrawer.enabled ? 3 : 4

        actions: [
            Action {
                iconName: "image/color_lens"
                name: "Colors"
                onTriggered: colorPicker.show()
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
               //onTriggered: window.showError(selectedProf.toString(), "Do you want to retry?", "Close", true)
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
                hoverAnimation: true
            },

            Action {
                iconName: "content/clear"
                name: "Exit"
                hoverAnimation: true
                onTriggered: manager.exitTriggered()
            }
        ]


//        Component {
//            id: tabComponent
        Repeater {
            id: tabRep
            model: sectionTitles
            Tab {
                title: sectionTitles[index]
                onActiveChanged:// {
                    selectedProf = index
//                    colomnRep.model = manager.getTimesList(selectedProf, selectedDay)
//                }
                Row {
                    anchors.fill: parent
                    Rectangle {
                        id: daysList
                        // left bar with weekdays
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        width: parent.width / 4
                        Flickable {
                            anchors.fill: parent
                            contentHeight: Math.max(daysView.implicitHeight, height)

                            View {
                                anchors.fill: parent
                                anchors.rightMargin: 15
                                elevation:  3
                                Column {
                                    id: daysView
                                    anchors.fill: parent

                                    Repeater {
                                        model: days
                                        ListItem.Standard {
                                            id: dayListItem
                                            text: index == selectedDay ? "<b>"+days[index]+"<\b>" : days[index]

//                                            backgroundColor: index == selectedDay ? "#EEEEEE" : "white"
                                            onClicked: selectedDay = index
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

                        Flickable {
                            id: interFl
                            anchors.fill: parent
                            contentHeight:intervColumn.implicitHeight+30
                            View {
                                anchors.fill: parent
                                anchors.margins: 15
                                elevation: 3
                                Column {
                                    id: intervColumn
                                    anchors.fill: parent

                                    Repeater {
                                        id: colomnRep
                                        model: manager.getTimesList(selectedProf, selectedDay)
                                        ListItem.Subtitled {

                                            text: modelData
                                            subText: "dsfsdf"+selectedProf+ " "+selectedDay //manager.getSitesList(selectedProf, selectedDay)[index]
                                            maximumLineCount: 3
                                            secondaryItem: Icon {
                                                name: "action/alarm_off"
                                                anchors.verticalCenter: parent.verticalCenter
                                                visible: true
                                                // TODO!!^^^^ ! manager.blocked
                                                MouseArea {
                                                    anchors.fill: parent
                                                    onClicked:{
                                                        manager.deleteInterval(selectedDay, selectedDay, index)
                                                        colomnRep.model = manager.getTimesList(selectedProf, selectedDay)
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

//        }

//        Loader {
//            anchors.fill: parent
//            sourceComponent: tabComponent
//        }











    }

    Dialog {
        id: colorPicker
        title: "Pick color"

        positiveButtonText: "Done"

        MenuField {
            id: selection
            model: ["Primary color", "Accent color", "Background color"]
            width: Units.dp(160)
        }

        Grid {
            columns: 7
            spacing: Units.dp(8)

            Repeater {
                model: [
                    "red", "pink", "purple", "deepPurple", "indigo",
                    "blue", "lightBlue", "cyan", "teal", "green",
                    "lightGreen", "lime", "yellow", "amber", "orange",
                    "deepOrange", "grey", "blueGrey", "brown", "black",
                    "white"
                ]

                Rectangle {
                    width: Units.dp(30)
                    height: Units.dp(30)
                    radius: Units.dp(2)
                    color: Palette.colors[modelData]["500"]
                    border.width: modelData === "white" ? Units.dp(2) : 0
                    border.color: Theme.alpha("#000", 0.26)

                    Ink {
                        anchors.fill: parent

                        onPressed: {
                            switch(selection.selectedIndex) {
                                case 0:
                                    theme.primaryColor = parent.color
                                    break;
                                case 1:
                                    theme.accentColor = parent.color
                                    break;
                                case 2:
                                    theme.backgroundColor = parent.color
                                    break;
                            }
                        }
                    }
                }
            }
        }
        onRejected: {
             // TODO set default colors again but we currently don't know what that is
        }
    }

    Component {
        id: tabDelegate

        Item {
            Sidebar {
                id: sidebar

                expanded: !navDrawer.enabled

                Column {
                    width: parent.width

                    Repeater {
                        model: section
                        delegate: ListItem.Standard {
                            text: modelData
                            selected: modelData == selectedComponent
                            onClicked: selectedComponent = modelData
                        }
                    }
                }
            }

            // here is magic
            Flickable {
                id: flickable
                anchors {
                    left: sidebar.right
                    right: parent.right
                    top: parent.top
                    bottom: parent.bottom
                }
                clip: true
                contentHeight: Math.max(example.implicitHeight + 40, height)
                Loader {
                    id: example
                    anchors.fill: parent
                    asynchronous: true
                    visible: status == Loader.Ready
                    // selectedComponent will always be valid, as it defaults to the first component
                    source: {
                        if (navDrawer.enabled) {
                            return Qt.resolvedUrl("%1window.qml").arg(window.selectedComponent.replace(" ", ""))
                        } else {
                            return Qt.resolvedUrl("%1window.qml").arg(selectedComponent.replace(" ", ""))
                        }
                    }
                }

                ProgressCircle {
                    anchors.centerIn: parent
                    visible: example.status == Loader.Loading
                }
            }
            Scrollbar {
                flickableItem: flickable
            }
        }
    }
}
