import QtQuick 2.0
import QtQuick.Window 2.2
import QtLocation 5.6
import QtPositioning 5.6

Window {
    visible: true
    title: qsTr("map")
    width: 1000
    height: 800

    PositionSource {
        active: true
        onPositionChanged: {
            map.center = position.coordinate;
        }
    }


    Map {
        id: map
        anchors.fill: parent
        plugin: Plugin { name: "osm" }
        center: QtPositioning.coordinate(46.4701, 30.7251)
        zoomLevel: 14
    }

    MapCircle {
        color: "green"
        center: QtPositioning.coordinate(46.4701, 30.7251)
        radius: 1000
    }

    MapQuickItem {
        coordinate: QtPositioning.coordinate(46.4701, 30.7251)
        sourceItem: Image {
            id: image
            source: "qrc:/mapMarker.png"
        }
        anchorPoint.x: image.width / 2
        anchorPoint.y: image.height
    }
}
