# model
newdocument("Acoustic", "planar", "acoustic", 3, 2, "disabled", 5, 1, 2000, "harmonic", 1.0, 1.0, 0.0)

# boundaries
addboundary("Source", "acoustic_pressure", 0.01)
addboundary("Wall", "acoustic_normal_acceleration", 0)
addboundary("Matched boundary", "acoustic_matched_boundary", 0)

# materials
addmaterial("Vzduch", 1.25, 343)

# edges
addedge(-0.4, 0.05, 0.1, 0.2, 0, "Matched boundary")
addedge(0.1, -0.2, -0.4, -0.05, 0, "Matched boundary")
addedge(-0.4, 0.05, -0.4, -0.05, 0, "Matched boundary")
addedge(-0.18, -0.06, -0.17, -0.05, 90, "Source")
addedge(-0.17, -0.05, -0.18, -0.04, 90, "Source")
addedge(-0.18, -0.04, -0.19, -0.05, 90, "Source")
addedge(-0.19, -0.05, -0.18, -0.06, 90, "Source")
addedge(0.1, -0.2, 0.1, 0.2, 90, "Matched boundary")
addedge(0.03, 0.1, -0.04, -0.05, 90, "Wall")
addedge(-0.04, -0.05, 0.08, -0.04, 0, "Wall")
addedge(0.08, -0.04, 0.03, 0.1, 0, "Wall")

# labels
addlabel(-0.0814934, 0.0707097, 0, 0, "Vzduch")
addlabel(-0.181474, -0.0504768, 0, 0, "none")
addlabel(0.0314514, 0.0411749, 0, 0, "none")

# solve
zoombestfit()
solve()

# point valueqtcreator 2.2 ppa
point = pointresult(-0.084614, 0.053416)
testp = test("Acoustic pressure", point["p"], 0.003064)
testp_real = test("Acoustic pressure - real", point["p_real"], 0.002322)
testp_imag = test("Acoustic pressure - imag", point["p_imag"], 0.001999)
testI = test("Acoustic pressure", point["Lp"], 40.695085)
testw = test("Energy", point["w"], 3.192054e-11)
testLw = test("Energy level", point["Lw"], 15.04070)

# volume integral
volume = volumeintegral(0)
testPv_real = test("Pressure - real", volume["p_real"], -1.915211e-5)
testPv_imag = test("Pressure - imag", volume["p_imag"], -1.918928e-5)
testW = test("Energy", volume["W"], 5.179213e-12)
testLW = test("Energy level", volume["Lw"], 1.778767)

# surface integral 
surface = surfaceintegral(7)
testPs_real = test("Pressure - real", surface["p_real"], 3.079084e-4)
testPs_imag = test("Pressure - imag", surface["p_imag"], 4.437581e-5)

print("Test: Acoustic - planar: " + str(testp and testp_real and testp_imag and testI and testPv_real and testPv_imag and testPs_real and testPs_imag and testw and testLw and testW and testLW))