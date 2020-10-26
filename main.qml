import QtQuick 2.3
import QtQuick.Window 2.2

Window {
    visible: true
    title: qsTr("map")
    width: 1000
    height: 800

//    Image {
//        id: image
//        source: "https://astroliga.com/wp-content/uploads/2017/02/%D0%A1%D0%BE%D0%BB%D0%BD%D1%86%D0%B5.jpg"
//        anchors.fill: parent
//        fillMode: Image.PreserveAspectFit
//    }

    MapQml {
        width: 1000;
        height: 800;
    }
}

