.pragma library
.import BigInteger 2.0 as BI

function BigInteger(value) {
}

BigInteger.prototype.equals = function(other) {
    if (typeof other === "string") {
        return BI.BigInteger._equals(this.value, other);
    }

    return BI.BigInteger._equals(this.value, other.value);

}

BigInteger.prototype.toString = function() {
    return BI.BigInteger._toString(this.value);
}

BigInteger.prototype.multiply = function(other) {
    var ret = new BigInteger();

    ret.value = BI.BigInteger._multiply(this.value, other);
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
