import minimc

result = minimc.Loaders.getLoader(0).loadFromFile("../../minimc/tests/programs/minimc/array.mmc")

control = minimc.Controller(result)
control.boolCasts()
control.createAssertViolateLocations()
control.expandNonDeterministic()

program = control.getProgram()

builder = minimc.CPA.makeBuilder()

initialiserDesc = minimc.CPA.IntialiseDescr.FromProgram(program)
statemap = minimc.Interpreter.StateMap(builder.makeInitialState(initialiserDesc))

parser = minimc.Interpreter.Parser(statemap,builder.makeTransfer(program))

