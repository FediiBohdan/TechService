import QtQuick 2.3
import QtQuick.Window 2.0
import QtPositioning 5.3
import QtLocation 5.3

Item {
        PositionSource {
            active: true
            onPositionChanged: {
                console.log(position.coordinate);
            }
        }

        Plugin {
            id: mapPlugin
            name: "osm"
            }

        Map {
            anchors.fill: parent
            plugin: mapPlugin
            center: QtPositioning.coordinate(46.4701, 30.7251)
            zoomLevel: 14
        }
}




