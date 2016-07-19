require 'open3'

# read input data that is passed directly
input_data=ARGV[0].strip

# form the options string by reading envirnment variables
options = ""

options += " -s " + ENV["Skip"].strip
options += " -u " + ENV["OnlyAlign"].strip unless ENV["OnlyAlign"] == "all"
options += " -5 " + ENV["TrimLeft"].strip
options += " -3 " + ENV["TrimRight"].strip
options += " --phred64-quals" if ENV["PhredQuality"] == "64"
options += " --solexa-quals" if ENV["Solexa"] == "on"
options += " -n " + ENV["AlignN"].strip
options += " -v " + ENV["AlignV"].strip
options += " -n " + ENV["AlignN"].strip
options += " -e " + ENV["AlignE"].strip
options += " -l " + ENV["AlignL"].strip
options += " -I " + ENV["AlignI"].strip
options += " -X " + ENV["AlignX"].strip
options += " -k " + ENV["ReportK"].strip
options += " --all" if ENV["ReportAll"] == "on"
options += " -m " + ENV["ReportM"].strip unless ENV["ReportM"] == "no-limit"
options += " --best" if ENV["ReportBest"] == "on"
options += " --strata" if ENV["ReportStrata"] == "on"
options += " --suppress " + ENV["suppress"].delete(' ') unless ENV["suppress"] == "0"
options.strip!

# run the algorithm with the options injected
command = "./bowtie " + options + " e_coli " + input_data
stdin, stdout, stderr, wait_thr = Open3.popen3("echo '# " + command + "'; " + command)

# print the output to the standard output stream
puts stdout.read
puts stderr.read
