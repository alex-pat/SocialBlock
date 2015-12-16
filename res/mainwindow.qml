import QtQuick 2.4
import Material 0.1
import Material.ListItems 0.1 as ListItem
import SocialBlock.connector 1.0

ApplicationWindow {
    id: demo

    title: "SocialBlock"

    // Necessary when loading the window from C++
    visible: true

    SBConnector {
        id: manager
    }

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

    property var basicComponents: [
            "Button", "CheckBox", "Progress Bar", "Radio Button",
            "Slider", "Switch", "TextField"
    ]

    property var compoundComponents: [
            "Bottom Sheet", "Dialog", "Forms", "List Items", "Page Stack", "Time Picker", "Date Picker"
    ]

    Repeater {
        id: weekdays
        model: manager.getProfilesCount()
    }

    property var sections: [ days, days, days, days /*basicComponents, compoundComponents*/ ]

    property var sectionTitles: manager.getProfileNames()

    property string selectedComponent: styles[ manager.getCurrentProfileNumber() ]

    initialPage: TabbedPage {
        id: page

        title: "SocialBlock"

        actionBar.maxActionCount: 3 //navDrawer.enabled ? 3 : 4

        actions: [
            Action {
                iconName: "image/color_lens"
                name: "Colors"
                onTriggered: colorPicker.show()
            },

            Action {
                iconName: "action/settings"
                name: "Settings"
                hoverAnimation: true
            },

            Action {
                iconName: "awesome/user_plus"
                name: "Add profile"
                onTriggered: demo.showError("Something went wrong", "Do you want to retry?", "Close", true)
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
                iconName: "action/alarm_off"
                name: "Remove time interval"
            }
        ]

        backAction: navDrawer.action

        NavigationDrawer {
            id: navDrawer

            enabled: page.width < Units.dp(500)

            Flickable {
                anchors.fill: parent

                contentHeight: Math.max(content.implicitHeight, height)

                Column {
                    id: content
                    anchors.fill: parent

                    Repeater {
                        model: sections

                        delegate: Column {
                            width: parent.width

                            ListItem.Subheader {
                                text: sectionTitles[index]
                            }

                            Repeater {
                                model: modelData
                                delegate: ListItem.Standard {
                                    text: modelData
                                    selected: modelData == demo.selectedComponent
                                    onClicked: {
                                        demo.selectedComponent = modelData
                                        navDrawer.close()
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        Repeater {
            model: !navDrawer.enabled ? sections : 0

            delegate: Tab {
                title: sectionTitles[index]

                property string selectedComponent: modelData[0]
                property var section: modelData

                sourceComponent: tabDelegate
            }
        }

        Loader {
            anchors.fill: parent
            sourceComponent: tabDelegate

            property var section: []
            visible: navDrawer.enabled
        }
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
                            return Qt.resolvedUrl("%1Demo.qml").arg(demo.selectedComponent.replace(" ", ""))
                        } else {
                            return Qt.resolvedUrl("%1Demo.qml").arg(selectedComponent.replace(" ", ""))
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
