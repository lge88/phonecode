var Transform = require('stream').Transform;
var util = require('util');

// based on:
// http://strongloop.com/strongblog/practical-examples-of-the-new-node-js-streams-api/
function Liner(options) {
  if (!(this instanceof Liner)) {
    return new Liner(options);
  }

  options || (options = {});
  options.objectMode || (options.objectMode = true);
  options.transformLine || (options.transformLine = identity);

  Transform.call(this, options);

  this._transformLine = options.transformLine;
  this._lastLineData = null;
  return this;
}

function identity(x) { return x; }

util.inherits(Liner, Transform);

Liner.prototype._transform = function(chunk, encoding, done) {
  var data = this._lastLineData ?
        this._lastLineData + chunk.toString() :
        chunk.toString(),
      lines = data.split('\n'),
      self = this;
  this._lastLineData = lines.pop();

  lines
    .map(function(line) {
      return self._transformLine(line) + '\n';
    })
    .forEach(this.push.bind(this));
  done();
};

Liner.prototype._flush = function(done) {
  if (this._lastLineData) {
    this.push(this._transformLine(this._lastLineData) + '\n');
  }
  this._lastLineData = null;
  done();
};

exports.Liner = Liner;
