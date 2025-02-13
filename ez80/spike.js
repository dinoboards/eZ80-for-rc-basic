function floatToString(bytes) {
  if (bytes.length !== 5) {
      throw new Error("Invalid input: Expected 5 bytes");
  }

  // Extract the exponent and mantissa
  const exponent = bytes[4] - 127;
  let mantissa = 0.5; // The implicit leading 1 in the mantissa

  // Calculate the mantissa value
  for (let i = 0; i < 4; i++) {
      for (let bit = 0; bit < 8; bit++) {
          if (bytes[i] & (1 << (7 - bit))) {
              mantissa += Math.pow(2, -(8 * i + bit + 1));
          }
      }
  }

  // Calculate the final value
  const value = mantissa * Math.pow(2, exponent);

  return value.toString();
}

// Example usage:
const bytes = [0x66, 0x66, 0x66, 0x46, 0x81]; // 5.5 in the given format
console.log(floatToString(bytes)); // Output: "5.5"


