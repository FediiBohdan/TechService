import QtQuick 2.7
import QtQuick.Window 2.2
import QtLocation 5.6
import QtPositioning 5.6

Window {
    visible: true
    title: qsTr("Карта СТО")
    width: 1000
    height: 800

    Map {
        id: map
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(46.4701, 30.7251) //59.930243, 10.714635
        zoomLevel: 14
    }

    Plugin {
        id: mapPlugin
        name: "osm"
    }

    MapCircle {
        color: "green"
        center: QtPositioning.coordinate(46.4701, 30.7251)
        radius: 1000
    }

    MapQuickItem {
            anchorPoint.x: image.width
            anchorPoint.y: image.height
            coordinate:  QtPositioning.coordinate(46.4701, 30.7251)
            sourceItem: Image {
                id: image
                source: "qrc:/images/mapMarker.png"
            }
    }
}
