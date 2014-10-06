var fs = require('fs');

function slurp(filename) {
  return fs.readFileSync(filename, 'utf8');
}

function buildWordsList(wordsFile) {
  return slurp(wordsFile).trim().split('\n');
}

function buildLetterToDigitMap(phoneFile) {
  return slurp(phoneFile).trim().split('\n').map(function(line) {
    return line.trim().split(',');
  }).reduce(function(result, current) {
    var digit = current[0], letters = current[1];
    letters.split('').forEach(function(letter) {
      result[letter.toUpperCase()] = digit;
    });
    return result;
  }, {});
}

function wordToDigits(word, letterToDigitMap) {
  return word.split('').map(function(letter) {
    var digit = letterToDigitMap[letter.toUpperCase()];
    return digit ? digit : '';
  }).join('');
}

function buildDigitsToWordsMap(words, letterToDigitMap) {
  var out = {};
  words.forEach(function(word) {
    var digits = wordToDigits(word, letterToDigitMap);
    if (!out[digits]) {
      out[digits] = [];
    }
    out[digits].push(word);
  });
  return out;
}

function StringPartitionGenerator(str, start) {
  this._start = start || 0;
  this._cursor = this._start;
  this._str = str;
}

StringPartitionGenerator.STOP_ITERATION = {};

StringPartitionGenerator.prototype.reset = function() {
  this._cursor = this._start;
};

StringPartitionGenerator.prototype._partition = function(at) {
  return [this._str.slice(0, at), this._str.slice(at)];
};

StringPartitionGenerator.prototype.next = function() {
  if (this._cursor <= this._str.length) {
    return this._partition(this._cursor++);
  }
  return StringPartitionGenerator.STOP_ITERATION;
};

function decodeDigits(digits, digitsToWordsMap) {
  if (digits.length === 0)  {
    return [[]];
  }

  var partitionIter = new StringPartitionGenerator(digits, 1);
  var parts, head, rest;
  var firstWords, firstWord, partialSolutions;
  var out = [];

  while ((parts = partitionIter.next()) !== StringPartitionGenerator.STOP_ITERATION) {
    head = parts[0], rest = parts[1];
    firstWords = digitsToWordsMap[head];

    if (firstWords) {
      firstWords.forEach(function(firstWord) {
        partialSolutions = decodeDigits(rest, digitsToWordsMap);
        partialSolutions.forEach(function(partialSolution) {
          var solution = [firstWord].concat(partialSolution);
          out.push(solution);
        });
      });
    }
  }

  return out;
}

function Translator(wordsFile, phoneFile) {
  var words = buildWordsList(wordsFile);
  this.letterToDigitMap = buildLetterToDigitMap(phoneFile);
  this.digitsToWordsMap = buildDigitsToWordsMap(words, this.letterToDigitMap);
}

Translator.prototype.encode = function(word) {
  return word + ': ' + wordToDigits(word, this.letterToDigitMap);
};

Translator.prototype.decode = function(digits) {
  var sentences = decodeDigits(digits, this.digitsToWordsMap);
  return sentences.map(function(sentences) {
    return digits + ': ' + sentences.join(' ');
  }).join('\n');
};

exports.Translator = Translator;
