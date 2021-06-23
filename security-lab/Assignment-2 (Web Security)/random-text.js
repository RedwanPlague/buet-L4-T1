alphabet = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789'
alphalen = alphabet.length

function randInt(L) {
  return Math.floor(Math.random() * L)
}

function randomText() {
  var len = 15 + randInt(15)
  console.log(len)
  var text = ''
  for (var i = 0; i < len; i++) {
    text += alphabet[randInt(alphalen)]
  }
  return text
}

console.log(randomText())
