// Chrome protocol parser.
const stream = require('stream'),
			util = require('util'),
			HasarFiscalPrinter = require('hasar-fp');

Input = function Input() {
	stream.Transform.call(this);

	// Transform bytes...
	this._writableState.objectMode = false;
	// ...into objects.
	this._readableState.objectMode = true;

	// Unparsed data.
	this.buf = new Buffer(0);
	// Parsed length.
	this.len = null;
}

util.inherits(Input, stream.Transform);

Input.prototype._transform = function(chunk, encoding, done) {
	// Save this chunk.
	this.buf = Buffer.concat([ this.buf, chunk ]);

	var self = this;

	function parseBuf() {
		// Do we have a length yet?
		if (typeof self.len !== 'number') {
			// Nope. Do we have enough bytes for the length?
			if (self.buf.length >= 4) {
				// Yep. Parse the bytes.
				self.len = self.buf.readUInt32LE(0);
				// Remove the length bytes from the buffer.
				self.buf = self.buf.slice(4);
			}
		}

		// Do we have a length yet? (We may have just parsed it.)
		if (typeof self.len === 'number') {
			// Yep. Do we have enough bytes for the message?
			if (self.buf.length >= self.len) {
				// Yep. Slice off the bytes we need.
				var message = self.buf.slice(0, self.len);
				// Remove the bytes for the message from the buffer.
				self.buf = self.buf.slice(self.len);
				// Clear the length so we know we need to parse it again.
				self.len = null;
				// Parse the message bytes.
				var obj = JSON.parse(message.toString());
				// Enqueue it for reading.
				self.push(obj);
				// We could have more messages in the buffer so check again.
				parseBuf();
			}
		}
	}

	// Check for a parsable buffer (both length and message).
	parseBuf();

	// We're done.
	done();
}

Output = function Output() {
	stream.Transform.call(this);

	this._writableState.objectMode = true;
	this._readableState.objectMode = false;
}

util.inherits(Output, stream.Transform);

Output.prototype._transform = function(chunk, encoding, done) {
	var len = new Buffer(4);
	var buf = new Buffer(JSON.stringify(chunk));

	len.writeUInt32LE(buf.length, 0);

	this.push(len);
	this.push(buf);

	done();
}

Transform = function Transform(handler) {
	stream.Transform.call(this);

	this._writableState.objectMode = true;
	this._readableState.objectMode = true;

	this.handler = handler;
}

util.inherits(Transform, stream.Transform);

Transform.prototype._transform = function(msg, encoding, done) {
	this.handler(msg, this.push.bind(this), done);
}

// Do what we need do.
process.stdin
	.pipe(new Input())
	.pipe(new Transform(function(message, push, done) {
		var fiscalPrinter = new HasarFiscalPrinter(message.model);
		fiscalPrinter[message.action].apply(fiscalPrinter, message.params)
			.catch(function(error) {
				return error;
			})
			.then(function(data) {
				push(data);
				done();
			});
	}))
	.pipe(new Output())
	.pipe(process.stdout);