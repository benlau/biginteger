.pragma library
.import BigInteger 2.0 as BI

function BigInteger(value) {
}

BigInteger.prototype.equals = function(other) {
    return BI.BigInteger.equals(this.value, other);
}

BigInteger.prototype.toString = function() {
    return BI.BigInteger._toString(this.value);
}

BigInteger.prototype.multiply = function(other) {
    var ret = new BigInteger();

    ret.value = BI.BigInteger._multiply(this.value, other);
    return ret;
}

BigInteger.prototype.divide = function(other) {

    var ret = new BigInteger();

    ret.value = BI.BigInteger._divide(this.value, other);

    return ret;
}

BigInteger.prototype.isNegative = function() {
    return BI.BigInteger.isNegative(this);
}

BigInteger.prototype.compare = function(other) {
    return BI.BigInteger.compare(this, other);
}

BigInteger.prototype.greater = function(other) {
    return this.compare(other) > 0;
}

BigInteger.prototype.greaterOrEquals = function(other) {
    return this.compare(other) >= 0;
}

BigInteger.prototype.lesser = function(other) {
    return this.compare(other) < 0;
}

BigInteger.prototype.lesserOrEquals = function(other) {
    return this.compare(other) <= 0;
}

BigInteger.prototype.add = function(other) {
    var ret = new BigInteger();
    ret.value = BI.BigInteger._add(this, other);
    return ret;
}

BigInteger.prototype.minus = function(other) {
    var ret = new BigInteger();
    ret.value = BI.BigInteger._minus(this, other);
    return ret;
}

function create(value) {
    var ret = new BigInteger();
    if (typeof value === "string" ||
        typeof value === "number") {
        ret.value = BI.BigInteger._createValue(value);
    } else {
        ret.value = value;
    }
    return ret;
}
