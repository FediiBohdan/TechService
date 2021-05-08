# TechService
This is the app for car service management. It supports orders, clients, spare parts, employees and tasks interaction.

## IDE ##
Qt Creator 4.14.1, compiler MCVS2019 32bit

## User guide ##
App consist of eighteen windows. Some of them will be shown and described.
The first window user sees is the main window, where are provided orders and tasks tables for quick access.
All other windows are available from main window. There are also links to service web-pages.<br><br>
![](https://github.com/FediiBohdan/TechService/blob/master/images/mainWindow.png) <br>

Then user should visit settings window, where DB and personal settings should be specified.
User authentication is required, as it provides access to application functionality.<br><br>
![](https://github.com/FediiBohdan/TechService/blob/master/images/settings.png) <br>

If the user has access right, he / she can create an order.
Client information, service, needed spare parts, employees and work list should be specified. If field is required, user will see corresponding tip. <br><br>
![](https://github.com/FediiBohdan/TechService/blob/master/images/orderCreation.png) <br>

All users can view orders list, where the most important information is shown. <br><br>
![](https://github.com/FediiBohdan/TechService/blob/master/images/orders.png) <br>

All users can view spare parts list. It is well-designed and easy to interact. Here will be shown default spare parts list. <br><br>
![](https://github.com/FediiBohdan/TechService/blob/master/images/sparePartsList.png) <br>

Spare parts list could be sorted by name or by car brand using corresponding button (for example, car brand - Mitsubishi). <br><br>
![](https://github.com/FediiBohdan/TechService/blob/master/images/sortedSpareParts.png) <br>

All users can add new spare part to DB. It is possible by means of corresponding window, where all fields are required. <br><br>
![](https://github.com/FediiBohdan/TechService/blob/master/images/addSparePart.png) <br>

Users with login 1XXX (Boss user type) can view car service analytics.
It is possible to view services profit, number of fulfilled orders, the most popular cars and spare parts. <br><br>
![](https://github.com/FediiBohdan/TechService/blob/master/images/analytics.png) <br>

## License ##
[MIT](https://opensource.org/licenses/mit-license.php)
