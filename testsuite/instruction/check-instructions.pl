#!/usr/bin/perl -w
# vi: set ts=4:
#

$debug = 0;

sub get_flags
{
	my @list = @insns;
	my $exts = {};
	my $s;

	$debug && print "function: $func\n";
	foreach $insn (@list) {
		if (grep { /^$insn$/ } @normal_list) {
			$debug && print "  $insn: normal\n";
		}elsif (grep { /^$insn$/ } @mmx_list) {
			$exts->{"mmx"} = 1;
			$debug && print "  $insn: mmx\n";
		}elsif (grep { /^$insn$/ } @mmx_ext_list) {
			$exts->{"mmxext"} = 1;
			$debug && print "  $insn: mmxext\n";
		}elsif (grep { /^$insn$/ } @_3dnow_list) {
			$exts->{"3dnow"} = 1;
			$debug && print "  $insn: 3dnow\n";
		}elsif (grep { /^$insn$/ } @_3dnow_ext_list) {
			$exts->{"3dnowext"} = 1;
			$debug && print "  $insn: 3dnowext\n";
		}elsif (grep { /^$insn$/ } @sse_list) {
			$exts->{"sse"} = 1;
			$debug && print "  $insn: sse\n";
		}elsif (grep { /^$insn$/ } @sse2_list) {
			$exts->{"sse2"} = 1;
			$debug && print "  $insn: sse2\n";
		}else {
			print "FIXME:\t\"$insn\",\n";
			$error = 1;
		}
	}
	$s = join(" ",sort(keys(%$exts)));
	$funcs->{"$func"} = $s;
	$debug && print "  FLAGS: $s\n";
}

sub check
{
	foreach $insn (@normal_list) {
		if (grep { /^$insn$/ } @mmx_list) {
			print "FIXME: $insn is in mmx_list\n";
			$error = 1;
		} elsif (grep { /^$insn$/ } @mmx_ext_list) {
			print "FIXME: $insn is in mmx_ext_list\n";
			$error = 1;
		} elsif (grep { /^$insn$/ } @_3dnow_list) {
			print "FIXME: $insn is in _3dnow_list\n";
			$error = 1;
		} elsif (grep { /^$insn$/ } @_3dnow_ext_list) {
			print "FIXME: $insn is in _3dnow_ext_list\n";
			$error = 1;
		} elsif (grep { /^$insn$/ } @sse_list) {
			print "FIXME: $insn is in sse_list\n";
			$error = 1;
		} elsif (grep { /^$insn$/ } @sse2_list) {
			print "FIXME: $insn is in sse2_list\n";
			$error = 1;
		}
	}
}

@normal_list = (
	"add", 
	"addl", 
	"and", 
	"andl", 
	"call", 
	"cld", 
	"cltd", 
	"cmovg", 
	"cmp", 
	"cmpb", 
	"cmpl", 
	"cwtl", 
	"dec", 
	"decl", 
	"fabs", 
	"fadd", 
	"faddl", 
	"faddp", 
	"fadds", 
	"fdivrl", 
	"fild", 
	"fildl", 
	"fildll", 
	"fistp", 
	"fistpl", 
	"fistpll", 
	"fldcw", 
	"fldl", 
	"flds", 
	"fldz", 
	"fmul", 
	"fmull", 
	"fmulp", 
	"fmuls", 
	"fnstcw", 
	"fnstsw", 
	"fsqrt", 
	"fstl", 
	"fstp", 
	"fstpl", 
	"fstps", 
	"fsts", 
	"fsub", 
	"fsubl", 
	"fsubp", 
	"fsubr", 
	"fsubrl", 
	"fsubrp", 
	"fucom", 
	"fucomp", 
	"fucompp", 
	"fxch", 
	"imul", 
	"inc", 
	"incl", 
	"ja", 
	"jae", 
	"jbe", 
	"je", 
	"jg", 
	"jge", 
	"jl", 
	"jle", 
	"jmp", 
	"jne", 
	"jns", 
	"jp", 
	"js", 
	"lea", 
	"leave", 
	"mov", 
	"movb", 
	"movl", 
	"movsbl", 
	"movsbw", 
	"movswl", 
	"movsww", 
	"movzbl", 
	"movzbw", 
	"movzwl", 
	"mulb", 
	"neg", 
	"nop", 
	"not", 
	"or", 
	"pop", 
	"push", 
	"pushl", 
	"repz", 
	"ret", 
	"rol", 
	"sahf", 
	"sar", 
	"sarl", 
	"shl", 
	"shr", 
	"sub", 
	"subl", 
	"test", 
	"testb", 
	"testl", 
	"xor", 
);

# verified
@mmx_list = (
	"emms",
	"movd",
	"movq",
	"packssdw",
	"packsswb",
	"packuswb",
	"paddb",
	"paddd",
	"paddsb",
	"paddsw",
	"paddusb",
	"paddusw",
	"paddw",
	"pand",
	"pandn",
	"pcmpeqb",
	"pcmpeqd",
	"pcmpgtb",
	"pcmpgtd",
	"pcmpgtw",
	"pmaddwd",
	"pmulhw",
	"pmullw",
	"por",
	"pslld",
	"psllq",
	"psllw",
	"psrad",
	"psraw",
	"psrld",
	"psrlq",
	"psrlw",
	"psubb",
	"psubd",
	"psubsb",
	"psubsw",
	"psubusb",
	"psubusw",
	"psubw",
	"punpckhbw",
	"punpckhdq",
	"punpckhwd",
	"punpcklbw",
	"punpckldq",
	"punpcklwd",
	"pxor"
);

# verified
@_3dnow_list = (
	"femms",
	"pavgusb",
	"pf2id",
	"pfacc",
	"pfadd",
	"pfcmpeq",
	"pfcmpge",
	"pfcmpgt",
	"pfmax",
	"pfmin",
	"pfmul",
	"pfrcp",
	"pfrcpit1",
	"pfrcpit2",
	"pfrsqit1",
	"pfrsqrt",
	"pfsub",
	"pfsubr",
	"pi2fd",
	"pmulhrw",
	"prefetch",
	"prefetchw"
);

# verified
@_3dnow_ext_list = (
	"pf2iw",
	"pfnacc",
	"pfpnacc",
	"pi2fw",
	"pswapd"
);

# verified
@mmx_ext_list = (
	"maskmovq",
	"movntq",
	"pavgb",
	"pavgw",
	"pextrw",
	"pinsrw",
	"pmaxsw",
	"pmaxub",
	"pminsw",
	"pminub",
	"pmovmskb",
	"pmulhuw",
	"prefetchnta",
	"prefetch0",
	"prefetch1",
	"prefetch2",
	"psadbw",
	"pshufw",
	"sfence"
);

# verified
@sse_list = (
	"addps",
	"addss",
	"andnps",
	"andps",
	"cmpps",
	"cmpss",
	"comiss",
	"cvtpi2ps",
	"cvtps2ps",
	"cvtsi2ss",
	"cvtss2si",
	"cvttps2pi",
	"cvttss2si",
	"divps",
	"divss",
	"fxrstor",
	"fxsave",
	"ldmxcsr",
	"maxps",
	"maxss",
	"minps",
	"minss",
	"movaps",
	"movhlps",
	"movhps",
	"movlhps",
	"movlps",
	"movmskps",
	"movss",
	"movups",
	"mulps",
	"mulss",
	"orps",
	"rcpps",
	"rcpss",
	"rsqrtps",
	"rsqrtss",
	"shufps",
	"sqrtps",
	"sqrtss",
	"stmxcsr",
	"subps",
	"subss",
	"ucomiss",
	"unpckhps",
	"unpcklps",
	"xorps"
);

@sse2_list = (
	"addpd",
	"addsd",
	"andnpd",
	"andpd",
	"cmppd",
	"cmpsd",
	"comisd",
	"cvtpi2pd",
	"cvtpd2pi",
	"cvtsi2sd",
	"cvtsd2si",
	"cvttpd2pi",
	"cvttsd2si",
	"cvtpd2ps",
	"cvtps2pd",
	"cvtsd2ss",
	"cvtss2sd",
	"cvtps2dq",
	"cvttpd2dq",
	"cvtdq2pd",
	"cvtps2dq",
	"cvttps2dq",
	"cvtdq2ps",
	"divpd",
	"divsd",
	"maxpd",
	"maxsd",
	"minsd",
	"movapd",
	"movhpd",
	"movlpd",
	"movmskpd",
	"movsd",
	"movupd",
	"mulpd",
	"mulsd",
	"orpd",
	"shufpd",
	"sqrtpd",
	"sqrtsd",
	"subpd",
	"subsd",
	"ucomisd",
	"unpckhpd",
	"unpcklpd",
	"xorpd",
);

$funcs = {};

$ARGV=shift @ARGV;
@output=`objdump -dr $ARGV`;

check();

$error = 0;
@insns = ();
while($_=shift @output){
	chomp;
	if(m/^0[0-9a-fA-F]+\s<[\.\w]+>:$/){
		$f = $_;
		$f =~ s/^0[0-9a-fA-F]+\s<([\.\w]+)>:$/$1/;

		if (@insns) {
			get_flags ();
		}

		$func = $f;

		@insns = ();
		$debug && print "$func:\n";

	} elsif(m/^[\s0-9a-f]+:\s[\s0-9a-f]+\s([a-z0-9]+)\s/){
		if (!grep { /$1/ } @insns) {
			push @insns, $1;
		}
		#print "  $1\n";
	} elsif (m/^$/) {
	} elsif (m/^Disassembly of section/) {
	} elsif (m/\sfile format\s/) {
	} else {
		print "FIXME: $_\n";
		$error = 1;
	}
}

@source = `./list-impls`;
while ($_ = shift @source) {
	chomp;
	if (m/^([\w\.]+):\s*([\w\s*]*)/) {
		$func = $1;
		$flags = $2;

		$xflags = $funcs->{$func};
		if (1) {
			if ($flags ne $xflags) {
				print "$func: \"$flags\" should be \"$xflags\"\n";
			}
		} else {
			print "FIXME: function \"$func\" has no disassembly\n";
			$error = 1;
		}
	} else {
		print "FIXME: bad match: $_\n";
	}
}

exit $error;

