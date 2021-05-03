import QtQuick 2.7
import QtQuick.Window 2.2
import QtLocation 5.6
import QtPositioning 5.6

Window
{
    visible: true
    visibility: "Maximized"
    title: qsTr("Карта СТО")

    property var locationCenter: QtPositioning.coordinate(46.4353855, 30.7063799)
    property var location1: QtPositioning.coordinate(46.455239, 30.7380335)
    property var location2: QtPositioning.coordinate(46.4475251, 30.6862126)
    property var location3: QtPositioning.coordinate(46.3849525, 30.7251305)

    Map
    {
        id: map
        anchors.fill: parent
        plugin: mapPlugin
        center: locationCenter
        zoomLevel: 13.15

        Plugin
        {
            id: mapPlugin
            name: "osm"
        }

        MapQuickItem
        {
            anchorPoint.x: image.width
            anchorPoint.y: image.height
            coordinate: location1
            sourceItem: Image
            {
                id: image
                source: "qrc:/images/mapMarker.png"
            }
        }

        MapQuickItem
        {
            anchorPoint.x: image2.width
            anchorPoint.y: image2.height
            coordinate: location2
            sourceItem: Image
            {
                id: image2
                source: "qrc:/images/mapMarker.png"
            }
        }

        MapQuickItem
        {
            anchorPoint.x: image3.width
            anchorPoint.y: image3.height
            coordinate: location3
            sourceItem: Image
            {
                id: image3
                source: "qrc:/images/mapMarker.png"
            }
        }
    }
}
