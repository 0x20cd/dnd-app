import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
//import QtQuick.Dialogs
import Qt.labs.platform
import AppEngineModule 1.0

ApplicationWindow {
    width: 495
    height: 880
    visible: true
    title: qsTr("DnD app")

    Material.theme: Material.Dark
    Material.accent: Material.Amber
    Material.primary: Material.Red

    header: ToolBar {
        RowLayout {
            anchors.fill: parent

            ToolButton {
                visible: stack.depth > 1
                text: qsTr("Back")
                implicitWidth: 100
                onClicked: stack.pop()
            }

            Item {
                Layout.fillWidth: true
            }

            ToolButton {
                visible: stack.depth === 1 && AppEngine.isPasswordSet && !AppEngine.isSignedIn
                text: qsTr("Change Master Password")
                onClicked: {
                    stack.push(setPasswordScreen)
                }
            }

            ToolButton {
                visible: AppEngine.isSignedIn
                text: qsTr("Exit") /*(AppEngine.isMaster ? qsTr("Master") : qsTr("Player")) + " | " + qsTr("Exit")*/
                implicitWidth: 100
                onClicked: {
                    AppEngine.signOut();
                    stack.clear(StackView.ReplaceTransition)
                    stack.push(signInScreen)
                }
            }
        }
    }

    StackView {
        id: stack
        anchors.fill: parent
        focus: true
        onFocusChanged: { focus = true; }

        Keys.onBackPressed: stack.pop()

        initialItem: AppEngine.isPasswordSet ? signInScreen : setPasswordScreen
    }

    Component {
        id: setPasswordScreen

        RowLayout {
            spacing: 0
            Item { implicitWidth: 10 }

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 20

                Item { Layout.fillHeight: true; }

                TextField {
                    id: oldPasswordTextField
                    visible: AppEngine.isPasswordSet
                    Layout.fillWidth: true
                    implicitHeight: 40
                    font.pixelSize: 20
                    echoMode: TextInput.Password
                    placeholderText: qsTr("Enter old password")
                    onAccepted: nextItemInFocusChain().focus = true //setPasswordButton.clicked()

                }

                TextField {
                    id: newPasswordTextField1
                    Layout.fillWidth: true
                    implicitHeight: 40
                    font.pixelSize: 20
                    echoMode: TextInput.Password
                    placeholderText: qsTr("Enter new password")
                    onAccepted: nextItemInFocusChain().focus = true //setPasswordButton.clicked()

                }

                TextField {
                    id: newPasswordTextField2
                    Layout.fillWidth: true
                    implicitHeight: 40
                    font.pixelSize: 20
                    echoMode: TextInput.Password
                    placeholderText: qsTr("Repeat new password")
                    onAccepted: setPasswordButton.clicked()

                }

                Button {
                    id: setPasswordButton
                    Layout.fillWidth: true
                    text: qsTr("Set password")
                    implicitHeight: 50

                    onClicked: {
                        if (newPasswordTextField1.text != newPasswordTextField2.text) {
                            passNotEqualMsg.open()
                            return
                        }

                        if (newPasswordTextField1.text === "") {
                            noPasswordMsg.open()
                            return
                        }

                        if (AppEngine.isPasswordSet) {
                            if (!AppEngine.changePassword(newPasswordTextField1.text, oldPasswordTextField.text)) {
                                oldPasswordIncorrectMsg.open()
                                return
                            }
                        } else {
                            AppEngine.changePassword(newPasswordTextField1.text)
                        }

                        oldPasswordTextField.clear()
                        newPasswordTextField1.clear()
                        newPasswordTextField2.clear()
                        stack.clear(StackView.ReplaceTransition)
                        stack.push(signInScreen)
                    }
                }

                Item { Layout.fillHeight: true; }
            }

            Item { implicitWidth: 10 }
        }
    }

    Component {
        id: signInScreen

        RowLayout {
            spacing: 0

            Item { implicitWidth: 10 }

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 20

                Item { Layout.fillHeight: true; }

                Button {
                    Layout.fillWidth: true

                    text: qsTr("Player")
                    implicitHeight: 70
                    onClicked: {
                        AppEngine.signIn()
                        stack.clear(StackView.ReplaceTransition)
                        stack.push(mainScreen)
                    }
                }

                Button {
                    Layout.fillWidth: true

                    text: qsTr("Master")
                    implicitHeight: 70
                    onClicked: {
                        stack.push(enterPasswordScreen)
                    }
                }

                Item { Layout.fillHeight: true; }
            }

            Item { implicitWidth: 10 }
        }
    }

    Component {
        id: enterPasswordScreen

        RowLayout {
            spacing: 0
            Item { implicitWidth: 10 }

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 20

                Item { Layout.fillHeight: true; }

                TextField {
                    id: passwordTextField
                    Layout.fillWidth: true
                    implicitHeight: 40
                    font.pixelSize: 20
                    echoMode: TextInput.Password
                    placeholderText: qsTr("Enter password")
                    onAccepted: signInButton.clicked()
                }

                Button {
                    id: signInButton
                    Layout.fillWidth: true
                    text: qsTr("Sign In")
                    implicitHeight: 50

                    onClicked: {
                        if (AppEngine.signIn(passwordTextField.text)) {
                            stack.clear(StackView.ReplaceTransition)
                            stack.push(mainScreen)
                        } else {
                            passwordIncorrectMsg.open()
                        }

                        passwordTextField.clear()
                    }
                }

                Item { Layout.fillHeight: true; }
            }

            Item { implicitWidth: 10 }
        }
    }

    Component {
        id: mainScreen
        Item {
            Page {
                anchors.fill: parent
                spacing: 0

                SwipeView {
                    id: swipeMain
                    currentIndex: tabsMain.currentIndex
                    anchors.fill: parent
                    /*Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.margins: 10*/
                    anchors.margins: 10
                    spacing: 20

                    MyListView {
                        model: AppEngine.settingModel
                        delegate: ItemDelegate {
                            width: parent.width
                            //Layout.fillWidth: true
                            text: model.ptr.display
                            onClicked: {
                                stack.push(settingScreen, {"setting" : model.ptr.createCopy() })
                            }
                            Row {
                                z: 1
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter

                                RoundButton {
                                    icon.source: "qrc:/res/file_copy.svg"
                                    enabled: AppEngine.isMaster
                                    onClicked: {
                                        stack.push(templateScreen, {"template" : AppEngine.createTemplate(model.ptr)});
                                    }
                                }
                                RoundButton {
                                    icon.source: "qrc:/res/delete.svg"
                                    enabled: AppEngine.isMaster
                                    onClicked: {
                                        AppEngine.settingModel.removeById(model.ptr.eid)
                                    }
                                }
                            }
                        }
                    }

                    MyListView {
                        model: AppEngine.templateModel
                        delegate: ItemDelegate {
                            //text: model.ptr.name + "    <%1>".arg(model.ptr.isValid ? "'%1'".arg(model.ptr.setting.display) : "[%1]".arg(model.ptr.settingHash))
                            text: "%1 | %2".arg(model.ptr.name)
                                           .arg(model.ptr.isValid
                                                ? "\"%1\"".arg(model.ptr.setting.display)
                                                : "[%1]".arg(model.ptr.settingHash))
                            width: parent.width
                            onClicked: {
                                if (model.ptr.isValid)
                                    stack.push(templateScreen, {"template" : model.ptr})
                                else
                                    characterIsInvalidMsg.open()
                            }

                            Row {
                                z: 1
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter

                                RoundButton {
                                    icon.source: "qrc:/res/file_copy.svg"
                                    onClicked: {
                                        if (model.ptr.isValid) {
                                            var charlist = AppEngine.generateCharlist(model.ptr)//model.ptr.generateCharlist()
                                            stack.push(charlistScreen, {"charlist" : charlist});
                                        } else
                                            characterIsInvalidMsg.open()
                                    }
                                }
                                RoundButton {
                                    icon.source: "qrc:/res/delete.svg"
                                    enabled: AppEngine.isMaster
                                    onClicked: {
                                        AppEngine.templateModel.remove(AppEngine.templateModel.getIndex(model.ptr))
                                    }
                                }
                            }
                        }
                    }

                    MyListView {
                        model: AppEngine.charlistModel
                        delegate: ItemDelegate {
                            width: parent.width
                            text: "%1 | %2".arg(model.ptr.name)
                                           .arg(model.ptr.isValid
                                                ? "\"%1\"".arg(model.ptr.setting.display)
                                                : "[%1]".arg(model.ptr.settingHash))

                            onClicked: {
                                if (model.ptr.isValid)
                                    stack.push(charlistScreen, {"charlist" : model.ptr})
                                else
                                    characterIsInvalidMsg.open()
                            }

                            Row {
                                z: 1
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter

                                RoundButton {
                                    enabled: AppEngine.isMaster
                                    icon.source: model.ptr.isLocked ? "qrc:/res/lock.svg" : "qrc:/res/lock_open.svg"
                                    onClicked: {
                                        model.ptr.isLocked = !model.ptr.isLocked
                                    }
                                }
                                RoundButton {
                                    enabled: AppEngine.isMaster || !model.ptr.isLocked
                                    icon.source: "qrc:/res/delete.svg"
                                    onClicked: {
                                        AppEngine.charlistModel.remove(AppEngine.charlistModel.getIndex(model.ptr))
                                    }
                                }
                            }
                        }
                    }
                }

                footer: TabBar {
                    id: tabsMain
                    currentIndex: swipeMain.currentIndex

                    //Layout.fillWidth: true
                    //contentHeight: 60

                    TabButton { text: qsTr("Settings") }
                    TabButton { text: qsTr("Templates") }
                    TabButton { text: qsTr("Charlists") }
                }
            }

            Column {
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.rightMargin: 20
                anchors.bottomMargin: 70
                spacing: 10
                enabled: AppEngine.isMaster

                z: 1
                RoundButton {
                    width: 80
                    height: width
                    //text: "NEW"
                    display: AbstractButton.IconOnly
                    icon.source: "qrc:/res/add.svg"
                    visible: tabsMain.currentIndex === 0
                    Material.background: Material.color(Material.primary, Material.Shade400)
                    onClicked: {
                        stack.push(settingScreen, {"setting" : AppEngine.settingModel.newObject(AppEngine)})
                    }
                }
                RoundButton {
                    width: 80
                    height: width
                    //text: "IMP"
                    display: AbstractButton.IconOnly
                    icon.source: "qrc:/res/upload.svg"
                    Material.background: Material.color(Material.primary, Material.Shade400)
                    onClicked: {
                        importFileDialog.open()
                        //AppEngine.settingModel.importFile()
                    }
                }

                FileDialog {
                    id: importFileDialog

                    onAccepted: {
                        if (swipeMain.currentItem !== undefined && !swipeMain.currentItem.model.importFile(currentFile))
                            failedImportMsg.open()
                    }
                }
            }
        }
    }

    Component {
        id: settingScreen

        Page {
            property var setting : undefined
            //property bool isApplied : false
            spacing: 0

            /*Component.onDestruction: {
                //setting.display = "DEBUG TEST"
                if (isApplied) {
                    if (!setting.saveFile()) {
                        cantSaveSettingMsg.open()
                    }
                    else if (!AppEngine.settingModel.append(setting)) {
                        setting.removeFile();
                    }
                }
            }*/

            SwipeView {
                id: swipeSetting
                currentIndex: tabsSetting.currentIndex
                /*Layout.fillHeight: true
                Layout.fillWidth: true*/
                anchors.fill: parent
                anchors.margins: 10
                spacing: 20

                // Overview
                ColumnLayout {
                    spacing: 15

                    Label { text: qsTr("Overview"); font.pixelSize: 30 }

                    ColumnLayout {
                        Layout.fillWidth: true
                        Label { text: qsTr("Name") + ":" }

                        TextField {
                            id: settingDisplay
                            enabled: AppEngine.isMaster
                            Layout.fillWidth: true
                            implicitHeight: 40
                            text: setting.display

                            Binding {
                                target: setting
                                property: "display"
                                value: settingDisplay.text
                            }
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        Label { text: qsTr("About") + ":" }

                        TextArea {
                            id: settingAbout
                            enabled: AppEngine.isMaster
                            Layout.fillWidth: true
                            implicitHeight: 200
                            text: setting.about

                            Binding {
                                target: setting
                                property: "about"
                                value: settingAbout.text
                            }
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        Label {
                            Layout.preferredWidth: 140
                            text: qsTr("Char Generator") + ":"
                        }
                        ComboBox {
                            Layout.fillWidth: true
                            enabled: AppEngine.isMaster
                            //id: itemItemRand
                            model: setting.randoms
                            valueRole: "ptr"
                            currentIndex: setting.genRand !== undefined ? setting.randoms.getIndex(setting.genRand) : -1
                            displayText:  currentValue !== undefined ? currentValue.display : "-"

                            delegate: ItemDelegate {
                                width: parent.width
                                text: model.ptr.display
                            }

                            onCurrentValueChanged: {
                                setting.genRand = currentValue
                            }
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        Label {
                            Layout.preferredWidth: 140
                            text: qsTr("Char Test Random") + ":"
                        }
                        ComboBox {
                            Layout.fillWidth: true
                            enabled: AppEngine.isMaster
                            model: setting.randoms
                            valueRole: "ptr"
                            currentIndex: setting.testRand !== undefined ? setting.randoms.getIndex(setting.testRand) : -1
                            displayText:  currentValue !== undefined ? currentValue.display : "-"

                            delegate: ItemDelegate {
                                width: parent.width
                                text: model.ptr.display
                            }

                            onCurrentValueChanged: {
                                setting.testRand = currentValue
                            }
                        }
                        Component.onCompleted: {
                            console.debug(123)
                            console.debug(123, setting.testRand.display)
                        }
                    }

                    Item { Layout.fillHeight: true }
                }

                // Chars
                ColumnLayout {
                    spacing: 15

                    Label { text: qsTr("Chars"); font.pixelSize: 30 }

                    ColumnLayout {
                        Layout.fillWidth: true
                        Label { text: qsTr("Name") + ":" }

                        RowLayout {
                            Layout.fillWidth: true
                            ComboBox {
                                Layout.fillWidth: true
                                id: charComboBox
                                model: setting.chars
                                valueRole: "ptr"
                                editText: currentValue !== undefined ? currentValue.display : "-"
                                displayText: editText
                                delegate: ItemDelegate {
                                    width: parent.width
                                    text: model.ptr.display
                                }

                                editable: AppEngine.isMaster && currentValue !== undefined
                                enabled: count > 0
                                onEditTextChanged: {
                                    if (charComboBox.editText !== "") {
                                        currentValue.display = charComboBox.editText
                                    }
                                }
                            }
                            RoundButton {
                                width: height
                                display: AbstractButton.IconOnly
                                icon.source: "qrc:/res/add.svg"
                                enabled: AppEngine.isMaster
                                onClicked: {
                                    setting.chars.append(compChar.createObject())
                                    charComboBox.currentIndex = charComboBox.count - 1
                                }
                            }
                            RoundButton {
                                width: height
                                display: AbstractButton.IconOnly
                                icon.source: "qrc:/res/delete.svg"
                                enabled: AppEngine.isMaster && charComboBox.currentValue !== undefined
                                onClicked: {
                                    if (!setting.chars.removeById(charComboBox.currentValue.eid))
                                        cantRemoveEntityMsg.open();
                                }
                            }
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        Label { text: qsTr("About") + ":" }

                        TextArea {
                            id: charAbout
                            Layout.fillWidth: true
                            implicitHeight: 100
                            text: charComboBox.currentValue ? charComboBox.currentValue.about : ""
                            enabled: AppEngine.isMaster && charComboBox.currentValue !== undefined
                            onTextChanged: {
                                if (charComboBox.currentValue !== undefined) {
                                    charComboBox.currentValue.about = text
                                }
                            }
                        }
                    }

                    Item { Layout.fillHeight: true }
                }

                // Randoms
                ColumnLayout {
                    spacing: 15

                    Label { text: qsTr("Randoms"); font.pixelSize: 30 }

                    ColumnLayout {
                        Layout.fillWidth: true
                        Label { text: qsTr("Name") + ":" }

                        RowLayout {
                            ComboBox {
                                Layout.fillWidth: true
                                id: randomComboBox
                                model: setting.randoms
                                valueRole: "ptr"
                                editText: currentValue !== undefined ? currentValue.display : "-"
                                displayText: editText
                                delegate: ItemDelegate {
                                    width: parent.width
                                    text: model.ptr.display
                                }

                                editable: AppEngine.isMaster && currentValue !== undefined
                                enabled: count > 0
                                onEditTextChanged: {
                                    if (randomComboBox.editText !== "") {
                                        currentValue.display = randomComboBox.editText
                                    }
                                }
                            }
                            RoundButton {
                                width: height
                                display: AbstractButton.IconOnly
                                icon.source: "qrc:/res/add.svg"
                                enabled: AppEngine.isMaster
                                onClicked: {
                                    setting.randoms.append(compRandom.createObject())
                                    randomComboBox.currentIndex = randomComboBox.count - 1
                                }
                            }
                            RoundButton {
                                width: height
                                display: AbstractButton.IconOnly
                                icon.source: "qrc:/res/delete.svg"
                                enabled: AppEngine.isMaster && randomComboBox.currentValue !== undefined
                                onClicked: {
                                    if (!setting.randoms.removeById(randomComboBox.currentValue.eid))
                                        cantRemoveEntityMsg.open();
                                }
                            }
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        Label { text: qsTr("About") + ":" }

                        TextArea {
                            id: randomAbout
                            Layout.fillWidth: true
                            implicitHeight: 100
                            text: randomComboBox.currentValue ? randomComboBox.currentValue.about : ""
                            enabled: AppEngine.isMaster && randomComboBox.currentValue !== undefined
                            onTextChanged: {
                                if (randomComboBox.currentValue !== undefined) {
                                    randomComboBox.currentValue.about = text
                                }
                            }
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        Label { text: qsTr("Distribution") + ":" }

                        RowLayout {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 40
                            Label {
                                Layout.fillWidth: true
                                Layout.preferredWidth: 100
                                verticalAlignment: Text.AlignVCenter
                                text: qsTr("Value")
                            }
                            Label {
                                Layout.fillWidth: true
                                Layout.preferredWidth: 100
                                verticalAlignment: Text.AlignVCenter
                                text: qsTr("Weight")
                            }
                            Item {Layout.preferredWidth: 50}
                        }

                        MyListView {
                            Layout.fillWidth: true
                            Layout.fillHeight: true

                            model: randomComboBox.currentValue ? randomComboBox.currentValue.dist : 0

                            delegate: RowLayout {
                                width: parent.width
                                TextField {
                                    Layout.fillWidth: true
                                    Layout.preferredWidth: 100
                                    text: model.value
                                    enabled: AppEngine.isMaster
                                    validator: IntValidator {}
                                    onTextChanged: {
                                        if (acceptableInput)
                                            model.value = text
                                    }
                                }
                                TextField {
                                    Layout.fillWidth: true
                                    Layout.preferredWidth: 100
                                    text: model.weight
                                    enabled: AppEngine.isMaster
                                    validator: IntValidator {bottom: 0}
                                    onTextChanged: {
                                        if (acceptableInput)
                                            model.weight = text
                                    }
                                }
                                RoundButton {
                                    icon.source: "qrc:/res/delete.svg"
                                    enabled: AppEngine.isMaster
                                    onClicked: {
                                        randomComboBox.currentValue.dist.remove(index)
                                    }
                                }
                            }
                        }

                        RoundButton {
                            //Layout.preferredWidth: 60
                            icon.source: "qrc:/res/add.svg"
                            display: AbstractButton.IconOnly
                            enabled: AppEngine.isMaster
                            onClicked: {
                                randomComboBox.currentValue.dist.append()
                            }
                        }
                    }
                }

                //Items
                ColumnLayout {
                    spacing: 15

                    Label { text: qsTr("Items"); font.pixelSize: 30 }

                    ColumnLayout {
                        Layout.fillWidth: true
                        Label { text: qsTr("Name") + ":" }

                        RowLayout {
                            ComboBox {
                                Layout.fillWidth: true
                                id: itemComboBox
                                model: setting.items
                                valueRole: "ptr"
                                editText: currentValue !== undefined ? currentValue.display : "-"
                                displayText: editText
                                delegate: ItemDelegate {
                                    width: parent.width
                                    text: model.ptr.display
                                }

                                editable: AppEngine.isMaster && currentValue !== undefined
                                enabled: count > 0
                                onEditTextChanged: {
                                    if (itemComboBox.editText !== "") {
                                        currentValue.display = itemComboBox.editText
                                    }
                                }
                            }
                            RoundButton {
                                width: height
                                display: AbstractButton.IconOnly
                                icon.source: "qrc:/res/add.svg"
                                enabled: AppEngine.isMaster
                                onClicked: {
                                    setting.items.append(compItem.createObject())
                                    itemComboBox.currentIndex = itemComboBox.count - 1
                                }
                            }
                            RoundButton {
                                width: height
                                display: AbstractButton.IconOnly
                                icon.source: "qrc:/res/delete.svg"
                                enabled: AppEngine.isMaster && itemComboBox.currentValue !== undefined
                                onClicked: {
                                    if (!setting.items.removeById(itemComboBox.currentValue.eid))
                                        cantRemoveEntityMsg.open();
                                }
                            }
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        Label { text: qsTr("About") + ":" }

                        TextArea {
                            id: itemAbout
                            Layout.fillWidth: true
                            implicitHeight: 100
                            text: itemComboBox.currentValue ? itemComboBox.currentValue.about : ""
                            enabled: AppEngine.isMaster && itemComboBox.currentValue !== undefined
                            onTextChanged: {
                                if (itemComboBox.currentValue !== undefined) {
                                    itemComboBox.currentValue.about = text
                                }
                            }
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        RowLayout {
                            Layout.fillWidth: true
                            Label {
                                Layout.preferredWidth: 140
                                text: qsTr("Char") + ":"
                            }
                            ComboBox {
                                Layout.fillWidth: true
                                //id: itemChar
                                model: setting.chars
                                valueRole: "ptr"
                                currentIndex: itemComboBox.currentValue !== undefined ? setting.chars.getIndex(itemComboBox.currentValue.chr) : -1
                                displayText: currentValue !== undefined ? currentValue.display : "-"
                                enabled: AppEngine.isMaster && itemComboBox.currentValue !== undefined

                                delegate: ItemDelegate {
                                    width: parent.width
                                    text: model.ptr.display
                                }

                                onCurrentValueChanged: {
                                    itemComboBox.currentValue.chr = currentValue
                                }
                            }
                        }

                        RowLayout {
                            Layout.fillWidth: true
                            Label {
                                Layout.preferredWidth: 140
                                text: qsTr("Item Random") + ":"
                            }
                            ComboBox {
                                Layout.fillWidth: true
                                //id: itemItemRand
                                model: setting.randoms
                                valueRole: "ptr"
                                currentIndex: itemComboBox.currentValue !== undefined ? setting.randoms.getIndex(itemComboBox.currentValue.itemRand) : -1
                                displayText: currentValue !== undefined ? currentValue.display : "-"
                                enabled: AppEngine.isMaster && itemComboBox.currentValue !== undefined

                                delegate: ItemDelegate {
                                    width: parent.width
                                    text: model.ptr.display
                                }

                                onCurrentValueChanged: {
                                    itemComboBox.currentValue.itemRand = currentValue
                                }
                            }
                        }

                        RowLayout {
                            Layout.fillWidth: true
                            Label {
                                Layout.preferredWidth: 140
                                text: qsTr("Quality Random") + ":"
                            }
                            ComboBox {
                                Layout.fillWidth: true
                                //id: itemItemRand
                                model: setting.randoms
                                valueRole: "ptr"
                                currentIndex: itemComboBox.currentValue !== undefined ? setting.randoms.getIndex(itemComboBox.currentValue.qualityRand) : -1
                                displayText: currentValue !== undefined ? currentValue.display : "-"
                                enabled: AppEngine.isMaster && itemComboBox.currentValue !== undefined

                                delegate: ItemDelegate {
                                    width: parent.width
                                    text: model.ptr.display
                                }

                                onCurrentValueChanged: {
                                    itemComboBox.currentValue.qualityRand = currentValue
                                }
                            }
                        }
                    }

                    Item { Layout.fillHeight: true }
                }

                // Skills
                ColumnLayout {
                    spacing: 15

                    Label { text: qsTr("Skills"); font.pixelSize: 30 }

                    ColumnLayout {
                        Layout.fillWidth: true
                        Label { text: qsTr("Name") + ":" }

                        RowLayout {
                            ComboBox {
                                Layout.fillWidth: true
                                id: skillComboBox
                                model: setting.skills
                                valueRole: "ptr"
                                editText: currentValue !== undefined ? currentValue.display : "-"
                                displayText: editText
                                delegate: ItemDelegate {
                                    width: parent.width
                                    text: model.ptr.display
                                }

                                editable: AppEngine.isMaster && currentValue !== undefined
                                enabled: count > 0
                                onEditTextChanged: {
                                    if (skillComboBox.editText !== "") {
                                        currentValue.display = skillComboBox.editText
                                    }
                                }

                                onCurrentIndexChanged: {
                                    console.debug(23, currentIndex)
                                }

                                onCurrentValueChanged: {
                                    console.debug(32, currentValue.display)
                                }

                                Component.onCompleted: {
                                    console.debug(443, currentIndex)
                                    console.debug(443, currentValue.display)
                                }
                            }
                            RoundButton {
                                width: height
                                display: AbstractButton.IconOnly
                                icon.source: "qrc:/res/add.svg"
                                enabled: AppEngine.isMaster
                                onClicked: {
                                    setting.skills.append(compSkill.createObject())
                                    skillComboBox.currentIndex = skillComboBox.count - 1
                                }
                            }
                            RoundButton {
                                width: height
                                display: AbstractButton.IconOnly
                                icon.source: "qrc:/res/delete.svg"
                                enabled: AppEngine.isMaster && skillComboBox.currentValue !== undefined
                                onClicked: {
                                    if (!setting.skills.removeById(skillComboBox.currentValue.eid))
                                        cantRemoveEntityMsg.open();
                                }
                            }
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        Label { text: qsTr("About") + ":" }

                        TextArea {
                            id: skillAbout
                            Layout.fillWidth: true
                            implicitHeight: 100
                            text: skillComboBox.currentValue ? skillComboBox.currentValue.about : ""
                            enabled: AppEngine.isMaster && skillComboBox.currentValue !== undefined
                            onTextChanged: {
                                if (skillComboBox.currentValue !== undefined) {
                                    skillComboBox.currentValue.about = text
                                }
                            }
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        RowLayout {
                            Layout.fillWidth: true
                            Label {
                                Layout.preferredWidth: 140
                                text: qsTr("Char") + ":"
                            }
                            ComboBox {
                                Layout.fillWidth: true
                                model: setting.chars
                                valueRole: "ptr"
                                currentIndex: skillComboBox.currentValue !== undefined ? setting.chars.getIndex(skillComboBox.currentValue.chr) : -1
                                displayText: currentValue !== undefined ? currentValue.display : "-"
                                enabled: AppEngine.isMaster && skillComboBox.currentValue !== undefined

                                delegate: ItemDelegate {
                                    width: parent.width
                                    text: model.ptr.display
                                }

                                onCurrentValueChanged: {
                                    skillComboBox.currentValue.chr = currentValue
                                }
                            }
                        }

                        RowLayout {
                            Layout.fillWidth: true
                            Label {
                                Layout.preferredWidth: 140
                                text: qsTr("PB factor") + " (%):"
                            }

                            TextField {
                                Layout.fillWidth: true
                                Layout.preferredWidth: 100
                                text: skillComboBox.currentValue.koeff
                                validator: IntValidator { bottom: 0 }
                                enabled: AppEngine.isMaster && skillComboBox.currentValue !== undefined
                                onTextChanged: {
                                    if (acceptableInput)
                                        skillComboBox.currentValue.koeff = text
                                }
                            }
                        }
                    }

                    Item { Layout.fillHeight: true }
                }

                // Races
                ColumnLayout {
                    spacing: 15

                    Label { text: qsTr("Races"); font.pixelSize: 30 }

                    ColumnLayout {
                        Layout.fillWidth: true
                        Label { text: qsTr("Name") + ":" }

                        RowLayout {
                            ComboBox {
                                Layout.fillWidth: true
                                id: raceComboBox
                                model: setting.races
                                valueRole: "ptr"
                                editText: currentValue !== undefined ? currentValue.display : "-"
                                displayText: editText
                                delegate: ItemDelegate {
                                    width: parent.width
                                    text: model.ptr.display
                                }

                                editable: AppEngine.isMaster && currentValue !== undefined
                                enabled: count > 0
                                onEditTextChanged: {
                                    if (raceComboBox.editText !== "") {
                                        currentValue.display = raceComboBox.editText
                                    }
                                }
                            }
                            RoundButton {
                                width: height
                                display: AbstractButton.IconOnly
                                icon.source: "qrc:/res/add.svg"
                                enabled: AppEngine.isMaster
                                onClicked: {
                                    setting.races.append(compRace.createObject())
                                    raceComboBox.currentIndex = raceComboBox.count - 1
                                }
                            }
                            RoundButton {
                                width: height
                                display: AbstractButton.IconOnly
                                icon.source: "qrc:/res/delete.svg"
                                enabled: AppEngine.isMaster && raceComboBox.currentValue !== undefined
                                onClicked: {
                                    if (!setting.races.removeById(raceComboBox.currentValue.eid))
                                        cantRemoveEntityMsg.open();
                                }
                            }
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        Label { text: qsTr("About") + ":" }

                        TextArea {
                            id: raceAbout
                            Layout.fillWidth: true
                            implicitHeight: 100
                            text: raceComboBox.currentValue ? raceComboBox.currentValue.about : ""
                            enabled: AppEngine.isMaster && raceComboBox.currentValue !== undefined
                            onTextChanged: {
                                if (raceComboBox.currentValue !== undefined) {
                                    raceComboBox.currentValue.about = text
                                }
                            }
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        Label { text: qsTr("Char bonus") + ":" }

                        MyListView {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 250

                            model: raceComboBox.currentValue !== undefined ? setting.chars : 0

                            delegate: RowLayout {
                                width: parent.width

                                Label {
                                    Layout.fillWidth: true
                                    Layout.preferredWidth: 100
                                    leftPadding: 20
                                    text: model.ptr.display
                                }

                                TextField {
                                    Layout.fillWidth: true
                                    Layout.preferredWidth: 100
                                    text: raceComboBox.currentValue.charBonus.get(model.ptr)
                                    validator: IntValidator { bottom: 0 }
                                    enabled: AppEngine.isMaster && raceComboBox.currentValue !== undefined
                                    onTextChanged: {
                                        if (acceptableInput)
                                            raceComboBox.currentValue.charBonus.set(model.ptr, text)
                                    }
                                    Connections {
                                        target: raceComboBox.currentValue.charBonus
                                        function onDataChanged() {
                                            text = raceComboBox.currentValue.charBonus.get(model.ptr)
                                        }
                                    }
                                }
                            }
                        }
                    }

                    Item { Layout.fillHeight: true }
                }

                // Classes
                ScrollView {
                    contentWidth: availableWidth
                    ColumnLayout {
                        width: parent.width
                        spacing: 15

                        Label { text: qsTr("Classes"); font.pixelSize: 30 }

                        ColumnLayout {
                            Layout.fillWidth: true
                            Label { text: qsTr("Name") + ":" }

                            RowLayout {
                                ComboBox {
                                    Layout.fillWidth: true
                                    id: classComboBox
                                    model: setting.classes
                                    valueRole: "ptr"
                                    editText: currentValue !== undefined ? currentValue.display : "-"
                                    displayText: editText
                                    delegate: ItemDelegate {
                                        width: parent.width
                                        text: model.ptr.display
                                    }

                                    editable: AppEngine.isMaster && currentValue !== undefined
                                    enabled: count > 0
                                    onEditTextChanged: {
                                        if (classComboBox.editText !== "") {
                                            currentValue.display = classComboBox.editText
                                        }
                                    }
                                }
                                RoundButton {
                                    width: height
                                    display: AbstractButton.IconOnly
                                    icon.source: "qrc:/res/add.svg"
                                    enabled: AppEngine.isMaster
                                    onClicked: {
                                        setting.classes.append(compClass.createObject())
                                        classComboBox.currentIndex = classComboBox.count - 1
                                    }
                                }
                                RoundButton {
                                    width: height
                                    display: AbstractButton.IconOnly
                                    icon.source: "qrc:/res/delete.svg"
                                    enabled: AppEngine.isMaster && classComboBox.currentValue !== undefined
                                    onClicked: {
                                        if (!setting.classes.removeById(classComboBox.currentValue.eid))
                                            cantRemoveEntityMsg.open();
                                    }
                                }
                            }
                        }

                        ColumnLayout {
                            Layout.fillWidth: true
                            Label { text: qsTr("About") + ":" }

                            TextArea {
                                id: classAbout
                                Layout.fillWidth: true
                                implicitHeight: 100
                                text: classComboBox.currentValue ? classComboBox.currentValue.about : ""
                                enabled: AppEngine.isMaster && classComboBox.currentValue !== undefined
                                onTextChanged: {
                                    if (classComboBox.currentValue !== undefined) {
                                        classComboBox.currentValue.about = text
                                    }
                                }
                            }
                        }

                        ColumnLayout {
                            Layout.fillWidth: true
                            RowLayout {
                                Layout.fillWidth: true
                                Label {
                                    Layout.preferredWidth: 140
                                    text: qsTr("Money Random") + ":"
                                }
                                ComboBox {
                                    Layout.fillWidth: true
                                    model: setting.randoms
                                    valueRole: "ptr"
                                    currentIndex: classComboBox.currentValue !== undefined ? setting.randoms.getIndex(classComboBox.currentValue.moneyRand) : -1
                                    displayText: currentValue !== undefined ? currentValue.display : "-"
                                    enabled: AppEngine.isMaster && classComboBox.currentValue !== undefined

                                    delegate: ItemDelegate {
                                        width: parent.width
                                        text: model.ptr.display
                                    }

                                    onCurrentValueChanged: {
                                        classComboBox.currentValue.moneyRand = currentValue
                                    }
                                }
                            }
                        }

                        ColumnLayout {
                            Layout.fillWidth: true
                            Label { text: qsTr("Skills") + ":" }

                            MyListView {
                                Layout.fillWidth: true
                                Layout.preferredHeight: 250

                                model: classComboBox.currentValue !== undefined ? classComboBox.currentValue.skills : 0

                                delegate: RowLayout {
                                    width: parent.width

                                    Label {
                                        leftPadding: 20
                                        Layout.fillWidth: true
                                        text: model.ptr.display
                                    }

                                    RoundButton {
                                        icon.source: "qrc:/res/delete.svg"
                                        enabled: AppEngine.isMaster
                                        onClicked: {
                                            classComboBox.currentValue.skills.remove(index)
                                        }
                                    }
                                }
                            }

                            RowLayout {
                                Layout.fillWidth: true

                                ComboBox {
                                    id: skillPick
                                    Layout.fillWidth: true
                                    model: setting.skills
                                    valueRole: "ptr"
                                    displayText: currentValue !== undefined ? currentValue.display : "-"
                                    enabled: classComboBox.currentValue !== undefined
                                    delegate: ItemDelegate {
                                        width: parent.width
                                        text: model.ptr.display
                                    }
                                }

                                RoundButton {
                                    icon.source: "qrc:/res/add.svg"
                                    enabled: AppEngine.isMaster && classComboBox.currentValue !== undefined
                                    onClicked: {
                                        classComboBox.currentValue.skills.append(skillPick.currentValue)
                                    }
                                }
                            }
                        }

                        ColumnLayout {
                            Layout.fillWidth: true
                            Label { text: qsTr("Items") + ":" }

                            MyListView {
                                Layout.fillWidth: true
                                Layout.preferredHeight: 250

                                model: classComboBox.currentValue !== undefined ? classComboBox.currentValue.items : 0

                                delegate: RowLayout {
                                    width: parent.width

                                    Label {
                                        leftPadding: 20
                                        Layout.fillWidth: true
                                        text: model.ptr.display
                                    }

                                    RoundButton {
                                        icon.source: "qrc:/res/delete.svg"
                                        enabled: AppEngine.isMaster
                                        onClicked: {
                                            classComboBox.currentValue.items.remove(index)
                                        }
                                    }
                                }
                            }

                            RowLayout {
                                Layout.fillWidth: true

                                ComboBox {
                                    id: itemPick
                                    Layout.fillWidth: true
                                    model: setting.items
                                    valueRole: "ptr"
                                    displayText: currentValue !== undefined ? currentValue.display : "-"
                                    enabled: classComboBox.currentValue !== undefined
                                    delegate: ItemDelegate {
                                        width: parent.width
                                        text: model.ptr.display
                                    }
                                }

                                RoundButton {
                                    icon.source: "qrc:/res/add.svg"
                                    enabled: AppEngine.isMaster && classComboBox.currentValue !== undefined
                                    onClicked: {
                                        classComboBox.currentValue.items.append(itemPick.currentValue)
                                    }
                                }
                            }
                        }

                        Item { Layout.fillHeight: true }
                    }
                }

                // Levels
                ColumnLayout {
                    spacing: 15

                    Label { text: qsTr("Levels"); font.pixelSize: 30 }

                    RowLayout {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 40
                        Label {
                            Layout.fillWidth: true
                            Layout.preferredWidth: 100
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            text: "Lvl"
                        }
                        Label {
                            Layout.fillWidth: true
                            Layout.preferredWidth: 100
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            text: "d(XP)"
                        }
                        Label {
                            Layout.fillWidth: true
                            Layout.preferredWidth: 100
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            text: "d(PB)"
                        }
                        Label {
                            Layout.fillWidth: true
                            Layout.preferredWidth: 100
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            text: "XP"
                        }
                        Label {
                            Layout.fillWidth: true
                            Layout.preferredWidth: 100
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            text: "PB"
                        }
                    }

                    MyListView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        model: setting.levels

                        delegate: RowLayout {
                            width: parent.width
                            Label {
                                Layout.fillWidth: true
                                Layout.preferredWidth: 100
                                text: index + 2
                                horizontalAlignment: Text.AlignHCenter
                            }

                            TextField {
                                Layout.fillWidth: true
                                Layout.preferredWidth: 100
                                text: model.xpDiff
                                validator: IntValidator { bottom: 1 }
                                enabled: AppEngine.isMaster
                                onTextChanged: {
                                    if (acceptableInput)
                                        model.xpDiff = text
                                }
                            }
                            TextField {
                                Layout.fillWidth: true
                                Layout.preferredWidth: 100
                                text: model.pbDiff
                                validator: IntValidator { bottom: 0 }
                                enabled: AppEngine.isMaster
                                onTextChanged: {
                                    if (acceptableInput)
                                        model.pbDiff = text
                                }
                            }
                            Label {
                                Layout.fillWidth: true
                                Layout.preferredWidth: 100
                                text: model.xp
                                horizontalAlignment: Text.AlignHCenter
                            }
                            Label {
                                Layout.fillWidth: true
                                Layout.preferredWidth: 100
                                text: model.pb
                                horizontalAlignment: Text.AlignHCenter
                            }
                        }
                    }

                    RoundButton {
                        icon.source: "qrc:/res/add.svg"
                        enabled: AppEngine.isMaster
                        display: AbstractButton.IconOnly
                        onClicked: {
                            setting.levels.insert()
                        }
                    }
                }
            }

            Row {
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.margins: 5
                z: 1

                RoundButton {
                    enabled: AppEngine.isMaster
                    width: 50
                    height: width
                    display: AbstractButton.IconOnly
                    icon.source: "qrc:/res/done.svg"
                    onClicked: {
                        //isApplied = true
                        if (!setting.saveFile()) {
                            cantSaveSettingMsg.open()
                        }
                        else {
                            if (!AppEngine.settingModel.append(setting)) {
                                setting.removeFile();
                                suchSettingExistsMsg.open();
                            }
                            stack.pop()
                        }
                    }
                }

                RoundButton {
                    width: 50
                    height: width
                    display: AbstractButton.IconOnly
                    icon.source: "qrc:/res/clear.svg"
                    onClicked: {
                        stack.pop()
                    }
                }
            }

            footer: TabBar {
                id: tabsSetting
                currentIndex: swipeSetting.currentIndex

                //Layout.fillWidth: true
                //contentHeight: 60

                TabButton { icon.source: "qrc:/res/home.svg"; display: AbstractButton.IconOnly } // Overview
                TabButton { icon.source: "qrc:/res/description.svg"; display: AbstractButton.IconOnly } // Chars
                TabButton { icon.source: "qrc:/res/casino.svg"; display: AbstractButton.IconOnly } // Randoms
                TabButton { icon.source: "qrc:/res/backpack.svg"; display: AbstractButton.IconOnly } // Items
                TabButton { icon.source: "qrc:/res/bolt.svg"; display: AbstractButton.IconOnly } // Skills
                TabButton { icon.source: "qrc:/res/face.svg"; display: AbstractButton.IconOnly } // Races
                TabButton { icon.source: "qrc:/res/class.svg"; display: AbstractButton.IconOnly } // Classes
                TabButton { icon.source: "qrc:/res/moving.svg"; display: AbstractButton.IconOnly } // Levels
            }
        }
    }

    Component {
        id: templateScreen

        Page {
            property var template : undefined
            spacing: 0

            Component.onDestruction: {
                template.readFile()
            }

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 15

                Label { text: qsTr("Template"); font.pixelSize: 30 }

                ColumnLayout {
                    Layout.fillWidth: true
                    Label { text: qsTr("Name") + ":" }

                    TextField {
                        id: templateName
                        Layout.fillWidth: true
                        implicitHeight: 40
                        text: template.name
                        enabled: AppEngine.isMaster

                        Binding {
                            target: template
                            property: "name"
                            value: templateName.text
                        }
                    }
                }

                ColumnLayout {
                    Layout.fillWidth: true
                    Label { text: qsTr("Description") + ":" }

                    TextArea {
                        id: templateDesc
                        Layout.fillWidth: true
                        implicitHeight: 200
                        text: template.desc
                        enabled: AppEngine.isMaster

                        Binding {
                            target: template
                            property: "desc"
                            value: templateDesc.text
                        }
                    }
                }

                ColumnLayout {
                    RowLayout {
                        Layout.fillWidth: true
                        Label {
                            Layout.preferredWidth: 140
                            text: qsTr("Race") + ":"
                        }
                        ComboBox {
                            Layout.fillWidth: true
                            model: template.setting.races
                            valueRole: "ptr"
                            currentIndex: template.race !== undefined ? template.setting.races.getIndex(template.race) : -1
                            displayText: currentValue !== undefined ? currentValue.display : "-"
                            enabled: AppEngine.isMaster

                            delegate: ItemDelegate {
                                width: parent.width
                                text: model.ptr.display
                            }

                            onCurrentValueChanged: {
                                template.race = currentValue
                            }
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        Label {
                            Layout.preferredWidth: 140
                            text: qsTr("Class") + ":"
                        }
                        ComboBox {
                            Layout.fillWidth: true
                            model: template.setting.classes
                            valueRole: "ptr"
                            currentIndex: template.cls !== undefined ? template.setting.classes.getIndex(template.cls) : -1
                            displayText: currentValue !== undefined ? currentValue.display : "-"
                            enabled: AppEngine.isMaster

                            delegate: ItemDelegate {
                                width: parent.width
                                text: model.ptr.display
                            }

                            onCurrentValueChanged: {
                                template.cls = currentValue
                            }
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        Label {
                            Layout.preferredWidth: 140
                            text: qsTr("Initial XP") + ":"
                        }
                        TextField {
                            Layout.fillWidth: true
                            Layout.preferredWidth: 100
                            text: template.xp
                            validator: IntValidator { bottom: 0 }
                            enabled: AppEngine.isMaster
                            onTextChanged: {
                                if (acceptableInput)
                                    template.xp = text
                            }
                        }
                    }
                }

                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
            }

            Row {
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.margins: 5
                z: 1

                RoundButton {
                    enabled: AppEngine.isMaster
                    width: 50
                    height: width
                    display: AbstractButton.IconOnly
                    icon.source: "qrc:/res/done.svg"
                    onClicked: {
                        if (!template.saveFile()) {
                            cantSaveTemplateMsg.open()
                        } else {
                            AppEngine.templateModel.append(template)
                            stack.pop()
                        }
                    }
                }

                RoundButton {
                    width: 50
                    height: width
                    display: AbstractButton.IconOnly
                    icon.source: "qrc:/res/clear.svg"
                    onClicked: {
                        stack.pop()
                    }
                }
            }
        }
    }

    Component {
        id: charlistScreen
        Page {
            id: charlistScreenPage
            property var charlist : undefined
            spacing: 0

            Component.onDestruction: {
                if (AppEngine.charlistModel.append(charlist)) {
                    charlist.isLocked = AppEngine.isMaster
                }

                if (AppEngine.isMaster || !charlist.isLocked) {
                    charlist.saveFile()
                }
            }

            SwipeView {
                id: swipeCharlist
                currentIndex: tabsCharlist.currentIndex
                /*Layout.fillHeight: true
                Layout.fillWidth: true*/
                anchors.fill: parent
                anchors.margins: 10
                spacing: 20

                // Overview
                ColumnLayout {
                    spacing: 15

                    Label { text: qsTr("Overview"); font.pixelSize: 30 }

                    ColumnLayout {
                        Layout.fillWidth: true
                        Label { text: qsTr("Name") + ":" }

                        TextField {
                            id: charlistName
                            Layout.fillWidth: true
                            implicitHeight: 40
                            text: charlist.name
                            enabled: AppEngine.isMaster || !charlist.isLocked

                            Binding {
                                target: charlist
                                property: "name"
                                value: charlistName.text
                            }
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        Label { text: qsTr("Description") + ":" }

                        TextArea {
                            id: charlistDesc
                            Layout.fillWidth: true
                            implicitHeight: 200
                            text: charlist.desc
                            enabled: AppEngine.isMaster || !charlist.isLocked

                            Binding {
                                target: charlist
                                property: "desc"
                                value: charlistDesc.text
                            }
                        }
                    }

                    ColumnLayout {
                        RowLayout {
                            Layout.fillWidth: true
                            Label {
                                Layout.preferredWidth: 2
                                Layout.fillWidth: true
                                horizontalAlignment: Text.AlignHCenter
                                text: qsTr("Race") + ":"
                            }
                            TextField {
                                Layout.preferredWidth: 5
                                Layout.fillWidth: true
                                enabled: false
                                color: Material.accentColor
                                text: charlist.race.display
                            }
                            Label {
                                Layout.preferredWidth: 2
                                Layout.fillWidth: true
                                horizontalAlignment: Text.AlignHCenter
                                text: qsTr("Class") + ":"
                            }
                            TextField {
                                Layout.preferredWidth: 5
                                Layout.fillWidth: true
                                enabled: false
                                color: Material.accentColor
                                text: charlist.cls.display
                            }
                        }

                        RowLayout {
                            Layout.fillWidth: true
                            Label {
                                Layout.preferredWidth: 3
                                Layout.fillWidth: true
                                horizontalAlignment: Text.AlignHCenter
                                text: qsTr("Level") + ":"
                            }
                            TextField {
                                Layout.preferredWidth: 3
                                Layout.fillWidth: true
                                enabled: false
                                color: Material.accentColor
                                text: charlist.level
                            }
                            Label {
                                Layout.preferredWidth: 2
                                Layout.fillWidth: true
                                horizontalAlignment: Text.AlignHCenter
                                text: qsTr("PB") + ":"
                            }
                            TextField {
                                Layout.preferredWidth: 3
                                Layout.fillWidth: true
                                enabled: false
                                color: Material.accentColor
                                text: charlist.pb
                            }
                            Label {
                                Layout.preferredWidth: 2
                                Layout.fillWidth: true
                                horizontalAlignment: Text.AlignHCenter
                                text: qsTr("XP") + ":"
                            }
                            TextField {
                                Layout.preferredWidth: 5
                                Layout.fillWidth: true
                                text: charlist.xp
                                validator: IntValidator { bottom: 0 }
                                enabled: AppEngine.isMaster || !charlist.isLocked
                                onTextChanged: {
                                    if (acceptableInput)
                                        charlist.xp = text
                                }
                            }
                        }

                        RowLayout {
                            Layout.fillWidth: true
                            Label {
                                Layout.preferredWidth: 2
                                Layout.fillWidth: true
                                horizontalAlignment: Text.AlignHCenter
                                text: qsTr("Money") + ":"
                            }
                            TextField {
                                Layout.preferredWidth: 5
                                Layout.fillWidth: true
                                text: charlist.money
                                validator: IntValidator { bottom: 0 }
                                enabled: AppEngine.isMaster || !charlist.isLocked
                                onTextChanged: {
                                    if (acceptableInput)
                                        charlist.money = text
                                }
                            }
                            Item {
                                Layout.preferredWidth: 7
                                Layout.fillWidth: true
                            }
                        }
                    }

                    Item {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                    }
                }

                // Chars
                ColumnLayout {
                    spacing: 15

                    Label { text: qsTr("Chars"); font.pixelSize: 30 }

                    MyListView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        model: charlist.setting.chars

                        delegate: RowLayout {
                            width: parent.width

                            Label {
                                Layout.fillWidth: true
                                Layout.preferredWidth: 100
                                leftPadding: 20
                                text: model.ptr.display
                            }

                            TextField {
                                Layout.fillWidth: true
                                Layout.preferredWidth: 100
                                color: Material.accentColor
                                enabled: false
                                text: charlist.chars[model.ptr.eid]
                            }

                            RoundButton {
                                icon.source: "qrc:/res/casino.svg"
                                onClicked: {
                                    testCharSkillPicker.createObject(charlistScreenPage,
                                                                     {"charlist" : charlist, "chr" : model.ptr})
                                }
                            }
                        }
                    }
                }

                // Items
                ColumnLayout {
                    spacing: 15

                    Label { text: qsTr("Items"); font.pixelSize: 30 }

                    MyListView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        model: charlist.items

                        delegate: RowLayout {
                            width: parent.width

                            Label {
                                Layout.fillWidth: true
                                Layout.preferredWidth: 100
                                leftPadding: 20
                                text: model.item.display
                            }

                            TextField {
                                Layout.fillWidth: true
                                Layout.preferredWidth: 100
                                color: Material.accentColor
                                enabled: false
                                text: model.quality + "%"
                            }

                            RoundButton {
                                icon.source: "qrc:/res/casino.svg"
                                onClicked: {
                                    thrownValueBox.text = charlist.useItem(model.item)
                                    thrownValueBox.visible = true
                                }
                            }

                            RoundButton {
                                icon.source: "qrc:/res/delete.svg"
                                enabled: AppEngine.isMaster || !charlist.isLocked
                                onClicked: {
                                    charlist.items.remove(index)
                                }
                            }
                        }
                    }

                    ColumnLayout {
                        RowLayout {
                            Label {
                                Layout.preferredWidth: 100
                                text: qsTr("Item") + ":"
                            }

                            ComboBox {
                                id: itemPickComboBox
                                Layout.fillWidth: true
                                model: charlist.setting.items //template.setting.races
                                valueRole: "ptr"
                                displayText: currentValue.display
                                enabled: AppEngine.isMaster || !charlist.isLocked

                                delegate: ItemDelegate {
                                    width: parent.width
                                    text: model.ptr.display
                                }
                            }
                        }
                        RowLayout {
                            Label {
                                Layout.preferredWidth: 100
                                text: qsTr("Quality") + " (%):"
                            }

                            TextField {
                                id: qualityTextField
                                validator: IntValidator { bottom: 0 }
                                enabled: AppEngine.isMaster || !charlist.isLocked
                                text: "0"
                                Layout.fillWidth: true
                            }

                            RoundButton {
                                icon.source: "qrc:/res/add.svg"
                                enabled: AppEngine.isMaster || !charlist.isLocked
                                onClicked: {
                                    charlist.items.append(itemPickComboBox.currentValue, qualityTextField.text)
                                }
                            }

                            RoundButton {
                                icon.source: "qrc:/res/casino.svg"
                                enabled: AppEngine.isMaster || !charlist.isLocked

                                onClicked: {
                                    charlist.items.append(itemPickComboBox.currentValue)
                                }
                            }
                        }
                    }
                }
            }

            Component {
                id: testCharSkillPicker

                Rectangle {
                    id: testCharSkillPickerRoot
                    property var charlist : undefined
                    property var chr : undefined

                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: 0.6 * Math.min(parent.width, parent.height)
                    height: width
                    color: Material.dialogColor
                    radius: 5

                    ButtonGroup {
                        id: buttonGroup
                    }

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 5
                        ScrollView {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            contentWidth: availableWidth
                            ColumnLayout {
                                id: radioButtonsBox
                            }
                        }

                        RowLayout {
                            Layout.fillWidth: true
                            Item { Layout.fillWidth: true }
                            Button {
                                text: qsTr("OK")
                                onClicked: {
                                    thrownValueBox.text = charlist.testChar(
                                                chr.eid,
                                                buttonGroup.checkedButton.skill)
                                    thrownValueBox.visible = true
                                    testCharSkillPickerRoot.destroy()
                                }
                            }

                            Button {
                                text: qsTr("Cancel")
                                onClicked: {
                                    testCharSkillPickerRoot.destroy()
                                }
                            }
                        }
                    }

                    Component.onCompleted:  {
                        skillRadioButton.createObject(radioButtonsBox, {"skill" : undefined, "checked" : true})

                        for (var i = 0; i < charlist.cls.skills.rowCount(); ++i) {
                            var skill = charlist.cls.skills.getByIndex(i)
                            console.debug(skill.chr.display);
                            if (skill.chr !== chr)
                                continue;
                            skillRadioButton.createObject(radioButtonsBox, {"skill" : skill})
                        }
                    }

                    Component {
                        id: skillRadioButton
                        RadioButton {
                            property var skill : undefined
                            ButtonGroup.group: buttonGroup
                            Layout.fillWidth: true
                            text: skill !== undefined ? skill.display : qsTr("None")
                        }
                    }
                }
            }

            MouseArea {
                id: thrownValueBox
                property alias text: thrownValueBoxLabel.text
                visible: false
                anchors.fill: parent
                Label {
                    id: thrownValueBoxLabel
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: 0.6 * Math.min(parent.width, parent.height)
                    height: width
                    background: Rectangle { color: Material.dialogColor; radius: 20 }
                    font.pixelSize: 45
                }
                onClicked: {
                    visible = false
                }
            }

            footer: TabBar {
                id: tabsCharlist
                currentIndex: swipeCharlist.currentIndex

                //Layout.fillWidth: true
                //contentHeight: 60

                TabButton { icon.source: "qrc:/res/home.svg"; display: AbstractButton.IconOnly } // Overview
                TabButton { icon.source: "qrc:/res/description.svg"; display: AbstractButton.IconOnly } // Chars
                //TabButton { icon.source: "qrc:/res/casino.svg"; display: AbstractButton.IconOnly } // Randoms
                TabButton { icon.source: "qrc:/res/backpack.svg"; display: AbstractButton.IconOnly } // Items
            }
        }
    }

    MessageDialog {
        id: suchSettingExistsMsg
        title: qsTr("Warning")
        text: qsTr("Such setting already exists - copy will not be saved!")
        buttons: MessageDialog.Ok
    }

    MessageDialog {
        id: passNotEqualMsg
        title: qsTr("Error")
        text: qsTr("The passwords are not equal!")
        buttons: MessageDialog.Ok
    }

    MessageDialog {
        id: noPasswordMsg
        title: qsTr("Error")
        text: qsTr("Please enter your password!")
        buttons: MessageDialog.Ok
    }

    MessageDialog {
        id: oldPasswordIncorrectMsg
        title: qsTr("Error")
        text: qsTr("Old password is incorrect!")
        buttons: MessageDialog.Ok
    }

    MessageDialog {
        id: passwordIncorrectMsg
        title: qsTr("Error")
        text: qsTr("Password is incorrect!")
        buttons: MessageDialog.Ok
    }

    MessageDialog {
        id: failedImportMsg
        title: qsTr("Error")
        text: qsTr("Failed to import this file!")
        buttons: MessageDialog.Ok
    }

    MessageDialog {
        id: cantSaveSettingMsg
        title: qsTr("Error")
        text: qsTr("Can't save changes: the setting is not valid!")
        buttons: MessageDialog.Ok
    }

    MessageDialog {
        id: cantRemoveEntityMsg
        title: qsTr("Error")
        text: qsTr("The entity can't be removed!")
        buttons: MessageDialog.Ok
    }

    MessageDialog {
        id: cantSaveTemplateMsg
        title: qsTr("Error")
        text: qsTr("Can't save changes: the template is not valid!")
        buttons: MessageDialog.Ok
    }

    MessageDialog {
        id: characterIsInvalidMsg
        title: qsTr("Error")
        text: qsTr("Setting doesn't exist!")
        buttons: MessageDialog.Ok
    }

    Component { id: compChar; DndChar { eid: AppEngine.getUniqueId(); display: "UNNAMED" } }
    Component { id: compRandom; DndRandom { eid: AppEngine.getUniqueId(); display: "UNNAMED" } }
    Component { id: compItem; DndItem { eid: AppEngine.getUniqueId(); display: "UNNAMED" } }
    Component { id: compSkill; DndSkill { eid: AppEngine.getUniqueId(); display: "UNNAMED"; koeff: 0 } }
    Component { id: compRace; DndRace { eid: AppEngine.getUniqueId(); display: "UNNAMED" } }
    Component { id: compClass; DndClass { eid: AppEngine.getUniqueId(); display: "UNNAMED" } }
}
