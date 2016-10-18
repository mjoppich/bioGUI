function tellme()
    io.write("This is coming from lua.tellme.\n")
end

function square(n)
    io.write("Within callfuncscript.lua fcn square, arg=")
    io.write(tostring(n))
    n = n * n
    io.write(", square=")
    io.write(tostring(n))
    print(".")
    return(n)
end

print("Priming run")