const exec = require('child_process').exec,
			fs = require('fs'),
			EOL = '\r\n';

function setJSONParams(params) {
	return new Promise(function(resolve, reject) {
		fs.writeFile(__dirname + '\\command.json', JSON.stringify(params), function(err) {
			if (err) {
				reject(err);
			} else {
				resolve();
			}
		});
	});
}

function callToDriver(params) {
	return setJSONParams(params).then(function() {
		return new Promise(function(resolve, reject) {
			exec(__dirname + '\\driver.exe', function (error, stdout, stderr) {
				if (error) {
					reject(error);
					return;
				}

				resolve(stdout);
			});
		});
	});
}

function HasarFiscalPrinter(model) {
	if (HasarFiscalPrinter.MODELS.indexOf(model) === -1) {
		throw new Error('Invalid Hasar fiscal printer model: ' + model);
	}

	this.model = model;
}

HasarFiscalPrinter.prototype.reporteX = function reporteX() {
	if (!HasarFiscalPrinter.canUseIn(this.model, 'REPORTE_X')) {
		throw new Error('This model can not use method reportX.');
	}

	return callToDriver({
		model: this.model,
		action: "REPORTE_X"
	}).then(function(result) {
		var rta = {};
		result = result.split(EOL);

		rta.numeroReporte                  = result.shift();
		rta.cantidadDFCancelados           = result.shift();
		rta.cantidadDNFHEmitidos           = result.shift();
		rta.cantidadDNFEmitidos            = result.shift();
		rta.cantidadDFEmitidos             = result.shift();
		rta.ultimoDocFiscalBC              = result.shift();
		rta.ultimoDocFiscalA               = result.shift();
		rta.montoVentasDocFiscal           = result.shift();
		rta.montoIVADocFiscal              = result.shift();
		rta.montoImpInternosDocFiscal      = result.shift();
		rta.montoPercepcionesDocFiscal     = result.shift();
		rta.montoIVANoInscriptoDocFiscal   = result.shift();
		rta.ultimaNotaCreditoBC            = result.shift();
		rta.ultimaNotaCreditoA             = result.shift();
		rta.montoVentasNotaCredito         = result.shift();
		rta.montoIVANotaCredito            = result.shift();
		rta.montoImpInternosNotaCredito    = result.shift();
		rta.montoPercepcionesNotaCredito   = result.shift();
		rta.montoIVANoInscriptoNotaCredito = result.shift();
		rta.ultimoRemito                   = result.shift();
		rta.cantidadNCCanceladas           = result.shift();
		rta.cantidadDFBCEmitidos           = result.shift();
		rta.cantidadDFAEmitidos            = result.shift();
		rta.cantidadNCBCEmitidas           = result.shift();
		rta.cantidadNCAEmitidas            = result.shift();

		return rta;
	})
}

HasarFiscalPrinter.prototype.reporteZ = function reporteZ(from, to) {
	if (!HasarFiscalPrinter.canUseIn(this.model, 'REPORTE_Z')) {
		throw new Error('This model can not use method reportZ.');
	}

	from = from.getFullYear().toString().substr(2,2) +
						 ('0' + (from.getMonth() + 1)).slice(-2) +
				     ('0' + from.getDate()).slice(-2);

	to = to.getFullYear().toString().substr(2,2) +
				 ('0' + (to.getMonth() + 1)).slice(-2) +
				 ('0' + to.getDate()).slice(-2);

	return callToDriver({
		model: this.model,
		action: "REPORTE_Z",
		from: from,
		to: to
	}).then(function(result) {
		return;
	});
}

HasarFiscalPrinter.prototype.ticketFiscal = function ticketFiscal(type, ticket) {
	if (!HasarFiscalPrinter.canUseIn(this.model, type)) {
		throw new Error('This model can not print this type of ticket.');
	}	
	
	ticket = {
		model   : this.model,
		action  : 'TICKET',
		type    : type,
		client  : {
			name   : ticket.client.name    || 'Consumidor final',
			id     : ticket.client.id      || '0000000000',
			idType : ticket.client.idType  || 'CUIT',
			address: ticket.client.address || '------',
			regimen: ticket.client.regimen || 'CONSUMIDOR_FINAL'
		},
		text    : ticket.text     || '',
		items: ticket.items.map(function formatItem(item, idx) {
			item = {
				title   : item.title    || 'Item ' + idx,
				price   : item.price    || 1,
				quantity: item.quantity || 1,
				iva     : item.iva      || 21,
				discount: item.discount || 0 
			}
			
			if (item.discount === 0) {
				delete item.discount;
			}
			
			return item;
		}),
		discount: ticket.discount || 0
	}
	
	if (ticket.discount === 0) {
		delete ticket.discount;
	}
	
	return callToDriver(ticket).then(function(result) {
		return result.split(EOL).shift();
	});
	
}

HasarFiscalPrinter.MODELS = ['615', '715'];

HasarFiscalPrinter.SUPPORTED_TOOLS = {
	REPORTE_X: {
		description: 'Generar reporte X',
		models: ['615', '715']
	},
	REPORTE_Z: {
		description: 'Generar reporte Z por rango de fechas',
		models: ['615', '715']
	},
	TICKET_FACTURA_A: {
		description: 'Generar ticket factura A',
		models: ['615', '715']
	},
	TICKET_FACTURA_B: {
		description: 'Generar ticket factura B',
		models: ['615', '715']
	}
};

HasarFiscalPrinter.canUseIn = function canUseIn(model, tool) {
	return HasarFiscalPrinter.SUPPORTED_TOOLS[tool].models.indexOf(model) !== -1;
}

module.exports = HasarFiscalPrinter;

// Sugar
HasarFiscalPrinter.whatCanIDo = function whatCanIDo(model) {
	const models = model ? (Array.isArray(model) ? model : [model]) : HasarFiscalPrinter.MODELS,
				lengthModels = models.length,
				tools = Object.keys(HasarFiscalPrinter.SUPPORTED_TOOLS),
				lengthTools = tools.length;

  console.log('┌─────────────────────────────────────────────────────────────┐');
  console.log('│■ WITH HASAR FISCAL PRINTER YOU...                           │');
	for(var nro=0; nro<lengthModels; nro++) {
		console.log('├─────────────────────────────────────────────────────────────┤');
    console.log('│■■ USING MODEL "' + models[nro] + '"...');
    console.log('│■■■■ CAN DO:');

		for(var tNro=0; tNro<lengthTools; tNro++) {
			if (HasarFiscalPrinter.canUseIn(models[nro], tools[tNro])) {
				console.log('│   » ' + HasarFiscalPrinter.SUPPORTED_TOOLS[tools[tNro]].description);
			}
		}

    console.log('│■■■■ CAN NOT DO:');
		for(var tNro=0; tNro<lengthTools; tNro++) {
			if (!HasarFiscalPrinter.canUseIn(models[nro], tools[tNro])) {
				console.log('    * ' + HasarFiscalPrinter.SUPPORTED_TOOLS[tools[tNro]].description);
			}
		}
	}
  console.log('└─────────────────────────────────────────────────────────────┘');
}