# hasar-fp
#### Driver para impresoras fiscales Hasar en Node.js y C++

Con este driver podemos manipular impresoras fiscales marca Hasar de una forma super practica.

### Features
* Promise based
* Non blocking
* Super easy and intuitive
* Free Software
* Portable (TO-DO)

### First touch in Windows CLI (Start to use in 20 seconds)
1. Download **driver.exe** and **command.json** files
2. Run **driver.exe**

*Note:* you need a fiscal printer model **715** connected or an fiscal emulator runing, for other models, edit **command.json** before run.

### First touch with node.js
```javascript
const HasarFiscalPrinter = require('hasar-fp');

// First argument is a fiscal printer model, for example: '715'
var fiscalPrinter = new HasarFiscalPrinter('715');

// Do something with fiscalPrinter...
```
#### Sugar! Auto-Documentation!

```javascript
HasarFiscalPrinter.whatCanIDo(); // First argument can be a model, for example: '715'
```

#### Report X
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

#### Report Z
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

### Under the hoods
This driver can be used by any language, not only node.js, the precompile binary "driver.exe" (comming soon for linux) can be called without any params, only require the file "command.json" in the same folder.
This file, is a common json config file, that contains for example:

##### For report X
```json
{  
   "model":"715",
   "action":"REPORTE_X"
}
```

##### For report Z
```json
{  
   "model":"715",
   "action":"REPORTE_Z",
   "from":"160910",
   "to":"161010"
}
```

##### For ticket
```json
{  
   "model":"715",
   "action":"TICKET",
   "type":"TICKET_FACTURA_B",
   "client":{  
      "name":"Pablo Nehuen Prados",
      "id":"20371209590",
      "idType":"CUIT",
      "address":"Los Andes 892",
      "regimen":"RESPONSABLE_INSCRIPTO"
   },
   "text":"Software: craving.com.ar",
   "items":[  
      {  
         "title":"Gaseosa",
         "price":100.25,
         "quantity":3,
         "iva":21,
         "discount":50
      },
      {  
         "title":"Golosinas",
         "price":20.55,
         "quantity":5,
         "iva":21
      },
      {  
         "title":"Encendedores",
         "price":20.55,
         "quantity":5,
         "iva":21,
         "discount":20
      }
   ],
   "discount":10
}
```
