ENTRY(_start)
phys = 0x8000;
SECTIONS
{
	.first_of_all phys : AT(phys)
	{
		*(.first_of_all*)
	}
	.rmode ALIGN(4096):
	{
		*(.rmode*)
	}

  .text ALIGN(0x1000) :
	{
		*(.text*)
		*(.gnu.linkonce.t*)
	}

	.rodata ALIGN(4096) :
	{
		start_ctors = .;
		*(.ctor*)
		end_ctors = .;

		start_dtors = .;
		*(.dtor*)
		end_dtors = .;

		*(.rodata*)
		*(.gnu.linkonce.r*)
	}
	
	.data ALIGN(4096) :
	{
		*(.data*)
		*(.gnu.linkonce.d*)
	}

	.bss ALIGN(4096) :
	{
		*(.COMMON*)
		*(.bss*)
		*(.gnu.linkonce.b*)
	}
	

  end = .;
}

