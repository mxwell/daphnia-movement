#! /usr/bin/ruby

Fields = ['X', 'V', 'x', 'v', 'e']

def gen_bin_operator(bin_operator)
	res = "return Point("
	res << Fields.map do |e| 
		"p." + e + " " + bin_operator + " q." + e
	end.join(", ")
	res + ");"
end

def bin_operation
	puts "enter sign of binary operator"
	sign = gets.chomp
	puts gen_bin_operator(sign)
end

def gen_suffix_operation
	puts "Enter suffix"
	suffix = gets.chomp
	res = "return Point("
	res << Fields.map do |e|
		"p." + e + " " + suffix
	end.join(", ")
	puts res + ");"
end

gen_suffix_operation