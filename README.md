# hasar-fp
#### Driver para impresoras fiscales Hasar en Node.js y C++

Con este driver podemos manipular impresoras fiscales marca Hasar de una forma super practica.

### Features
* Promise based
* Non blocking
* Super easy and intuitive
* Free Software
* Portable (TO-DO)

```javascript
const HasarFiscalPrinter = require('hasar-fp');

// Creamos una instancia del modelo que necesitemos, por ejemplo Hasar Fiscal Printer 715:
var fiscalPrinter = new HasarFiscalPrinter('715');

// Do something with fiscalPrinter...
```
### Sugar!

```javascript
HasarFiscalPrinter.whatCanIDo();
```

#### Reporte X
```javascript
fiscalPrinter.reporteX()
	.then(function(data) {
	  console.log('Reporte X success.');
	  console.log(data);
	})
	.catch(function(error) {
	  console.error('Reporte X failed.');
	  console.error(error);
	});
```

#### Reporte Z
```javascript
var from = new Date('2016/09/10'),
		to = new Date('2016/10/10');
		
fiscalPrinter.reporteZ(from, to)
	.then(function() {
	  console.log('Reporte Z success.');
	})
	.catch(function(error) {
	  console.error('Reporte Z failed.');
	  console.error(error);
	});
```

#### Ticket

```javascript
var ticket = {
	"text": "Software: craving.com.ar",
	"client": {
		"name": "Pablo Nehuen Prados",
		"id": "20371209590",
		"idType": "CUIT",
		"address": "Maipu 86",
		"regimen": "RESPONSABLE_INSCRIPTO"
	},
	"items": [
		{
			"title": "Gaseosa",
			"price": 20.25,
			"quantity": 3,
			"iva": 21,
			"discount": 50
		},
		{
			"title": "Golosinas",
			"price": 5.55,
			"iva": 21,
			"quantity": 5
		},
		{
			"title": "Encendedores",
			"price": 7.55,
			"quantity": 5,
			"iva": 21,
			"discount": 20
		}
	],
	"discount": 10
}

fiscalPrinter.ticketFiscal('TICKET_FACTURA_B', ticket)
	.then(function(nro) {
	  console.log('Ticket #' + nro + ' success.');
	})
	.catch(function(error) {
	  console.error('Ticket failed.');
	  console.error(error);
	});
```
