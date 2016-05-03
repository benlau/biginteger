.pragma library
.import BigInteger 1.0 as BI

function BigInteger(value) {
    this.value = BI.BigInteger._createValue(value);
}

BigInteger.prototype.equals = function(other) {
    return BI.BigInteger._equals(this.value, other.value);
}

BigInteger.prototype.toString = function() {
    return BI.BigInteger._toString(this.value);
}

function create(value) {
    return new BigInteger(value);
}
