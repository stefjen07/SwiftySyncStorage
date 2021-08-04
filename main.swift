import Foundation

let field = CField_new(cft_string, "Hello, world!")
let value = String(cString: field!.pointee.name)
print(value)
