#pragma once
#include <stdint.h>

namespace symbolic_access
{
	enum class CvSignature : uint32_t
	{
		/// First explicit signature.
		C7 = 1,
		// Signature indicating a C11 (VC 5.x) module info stream. Uses 32-bit types.
		C11 = 2,
		// Signature indicating a C13 (VC 7.x) module info stream. Uses zero terminated names.
		C13 = 4,
	};

	enum class Leaf : uint16_t
	{
		LF_VTSHAPE = 0x000a,
		LF_COBOL1 = 0x000c,
		LF_LABEL = 0x000e,
		LF_NULL = 0x000f,
		LF_NOTTRAN = 0x0010,
		LF_ENDPRECOMP = 0x0014,       // not referenced from symbol
		LF_TYPESERVER_ST = 0x0016,       // not referenced from symbol

		// leaf indices starting records but referenced only from type records

		LF_LIST = 0x0203,
		LF_REFSYM = 0x020c,

		LF_ENUMERATE_ST = 0x0403,

		// 32-bit type index versions of leaves, all have the 0x1000 bit set
		//
		LF_TI16_MAX = 0x1000,

		LF_MODIFIER = 0x1001,
		LF_POINTER = 0x1002,
		LF_ARRAY_ST = 0x1003,
		LF_CLASS_ST = 0x1004,
		LF_STRUCTURE_ST = 0x1005,
		LF_UNION_ST = 0x1006,
		LF_ENUM_ST = 0x1007,
		LF_PROCEDURE = 0x1008,
		LF_MFUNCTION = 0x1009,
		LF_COBOL0 = 0x100a,
		LF_BARRAY = 0x100b,
		LF_DIMARRAY_ST = 0x100c,
		LF_VFTPATH = 0x100d,
		LF_PRECOMP_ST = 0x100e,       // not referenced from symbol
		LF_OEM = 0x100f,       // oem definable type string
		LF_ALIAS_ST = 0x1010,       // alias (typedef) type
		LF_OEM2 = 0x1011,       // oem definable type string

		// leaf indices starting records but referenced only from type records

		LF_SKIP = 0x1200,
		LF_ARGLIST = 0x1201,
		LF_DEFARG_ST = 0x1202,
		LF_FIELDLIST = 0x1203,
		LF_DERIVED = 0x1204,
		LF_BITFIELD = 0x1205,
		LF_METHODLIST = 0x1206,
		LF_DIMCONU = 0x1207,
		LF_DIMCONLU = 0x1208,
		LF_DIMVARU = 0x1209,
		LF_DIMVARLU = 0x120a,

		LF_BCLASS = 0x1400,
		LF_VBCLASS = 0x1401,
		LF_IVBCLASS = 0x1402,
		LF_FRIENDFCN_ST = 0x1403,
		LF_INDEX = 0x1404,
		LF_MEMBER_ST = 0x1405,
		LF_STMEMBER_ST = 0x1406,
		LF_METHOD_ST = 0x1407,
		LF_NESTTYPE_ST = 0x1408,
		LF_VFUNCTAB = 0x1409,
		LF_FRIENDCLS = 0x140a,
		LF_ONEMETHOD_ST = 0x140b,
		LF_VFUNCOFF = 0x140c,
		LF_NESTTYPEEX_ST = 0x140d,
		LF_MEMBERMODIFY_ST = 0x140e,
		LF_MANAGED_ST = 0x140f,

		// Types w/ SZ names

		LF_ST_MAX = 0x1500,

		LF_TYPESERVER = 0x1501,       // not referenced from symbol
		LF_ENUMERATE = 0x1502,
		LF_ARRAY = 0x1503,
		LF_CLASS = 0x1504,
		LF_STRUCTURE = 0x1505,
		LF_UNION = 0x1506,
		LF_ENUM = 0x1507,
		LF_DIMARRAY = 0x1508,
		LF_PRECOMP = 0x1509,       // not referenced from symbol
		LF_ALIAS = 0x150a,       // alias (typedef) type
		LF_DEFARG = 0x150b,
		LF_FRIENDFCN = 0x150c,
		LF_MEMBER = 0x150d,
		LF_STMEMBER = 0x150e,
		LF_METHOD = 0x150f,
		LF_NESTTYPE = 0x1510,
		LF_ONEMETHOD = 0x1511,
		LF_NESTTYPEEX = 0x1512,
		LF_MEMBERMODIFY = 0x1513,
		LF_MANAGED = 0x1514,
		LF_TYPESERVER2 = 0x1515,
		LF_INTERFACE = 0x1519,
		LF_STRUCTURE32 = 0x1609,

		LF_NUMERIC = 0x8000,
		LF_CHAR = 0x8000,
		LF_SHORT = 0x8001,
		LF_USHORT = 0x8002,
		LF_LONG = 0x8003,
		LF_ULONG = 0x8004,
		LF_REAL32 = 0x8005,
		LF_REAL64 = 0x8006,
		LF_REAL80 = 0x8007,
		LF_REAL128 = 0x8008,
		LF_QUADWORD = 0x8009,
		LF_UQUADWORD = 0x800a,
		LF_COMPLEX32 = 0x800c,
		LF_COMPLEX64 = 0x800d,
		LF_COMPLEX80 = 0x800e,
		LF_COMPLEX128 = 0x800f,
		LF_VARSTRING = 0x8010,

		LF_OCTWORD = 0x8017,
		LF_UOCTWORD = 0x8018,

		LF_DECIMAL = 0x8019,
		LF_DATE = 0x801a,
		LF_UTF8STRING = 0x801b,

		LF_PAD0 = 0xf0,
		LF_PAD1 = 0xf1,
		LF_PAD2 = 0xf2,
		LF_PAD3 = 0xf3,
		LF_PAD4 = 0xf4,
		LF_PAD5 = 0xf5,
		LF_PAD6 = 0xf6,
		LF_PAD7 = 0xf7,
		LF_PAD8 = 0xf8,
		LF_PAD9 = 0xf9,
		LF_PAD10 = 0xfa,
		LF_PAD11 = 0xfb,
		LF_PAD12 = 0xfc,
		LF_PAD13 = 0xfd,
		LF_PAD14 = 0xfe,
		LF_PAD15 = 0xff,
	};

	enum class SymbolDefs : uint16_t
	{
		S_COMPILE = 0x0001,  // Compile flags symbol
		S_REGISTER_16t = 0x0002,  // Register variable
		S_CONSTANT_16t = 0x0003,  // constant symbol
		S_UDT_16t = 0x0004,  // User defined type
		S_SSEARCH = 0x0005,  // Start Search
		S_END = 0x0006,  // Block, procedure, "with" or thunk end
		S_SKIP = 0x0007,  // Reserve symbol space in $$Symbols table
		S_CVRESERVE = 0x0008,  // Reserved symbol for CV internal use
		S_OBJNAME_ST = 0x0009,  // path to object file name
		S_ENDARG = 0x000a,  // end of argument/return list
		S_COBOLUDT_16t = 0x000b,  // special UDT for cobol that does not symbol pack
		S_MANYREG_16t = 0x000c,  // multiple register variable
		S_RETURN = 0x000d,  // return description symbol
		S_ENTRYTHIS = 0x000e,  // description of this pointer on entry

		S_BPREL16 = 0x0100,  // BP-relative
		S_LDATA16 = 0x0101,  // Module-local symbol
		S_GDATA16 = 0x0102,  // Global data symbol
		S_PUB16 = 0x0103,  // a public symbol
		S_LPROC16 = 0x0104,  // Local procedure start
		S_GPROC16 = 0x0105,  // Global procedure start
		S_THUNK16 = 0x0106,  // Thunk Start
		S_BLOCK16 = 0x0107,  // block start
		S_WITH16 = 0x0108,  // with start
		S_LABEL16 = 0x0109,  // code label
		S_CEXMODEL16 = 0x010a,  // change execution model
		S_VFTABLE16 = 0x010b,  // address of virtual function table
		S_REGREL16 = 0x010c,  // register relative address

		S_BPREL32_16t = 0x0200,  // BP-relative
		S_LDATA32_16t = 0x0201,  // Module-local symbol
		S_GDATA32_16t = 0x0202,  // Global data symbol
		S_PUB32_16t = 0x0203,  // a public symbol (CV internal reserved)
		S_LPROC32_16t = 0x0204,  // Local procedure start
		S_GPROC32_16t = 0x0205,  // Global procedure start
		S_THUNK32_ST = 0x0206,  // Thunk Start
		S_BLOCK32_ST = 0x0207,  // block start
		S_WITH32_ST = 0x0208,  // with start
		S_LABEL32_ST = 0x0209,  // code label
		S_CEXMODEL32 = 0x020a,  // change execution model
		S_VFTABLE32_16t = 0x020b,  // address of virtual function table
		S_REGREL32_16t = 0x020c,  // register relative address
		S_LTHREAD32_16t = 0x020d,  // local thread storage
		S_GTHREAD32_16t = 0x020e,  // global thread storage
		S_SLINK32 = 0x020f,  // static link for MIPS EH implementation

		S_LPROCMIPS_16t = 0x0300,  // Local procedure start
		S_GPROCMIPS_16t = 0x0301,  // Global procedure start

		// if these ref symbols have names following then the names are in ST format
		S_PROCREF_ST = 0x0400,  // Reference to a procedure
		S_DATAREF_ST = 0x0401,  // Reference to data
		S_ALIGN = 0x0402,  // Used for page alignment of symbols

		S_LPROCREF_ST = 0x0403,  // Local Reference to a procedure
		S_OEM = 0x0404,  // OEM defined symbol

		// sym records with 32-bit types embedded instead of 16-bit
		// all have 0x1000 bit set for easy identification
		// only do the 32-bit target versions since we don't really
		// care about 16-bit ones anymore.
		S_TI16_MAX = 0x1000,

		S_REGISTER_ST = 0x1001,  // Register variable
		S_CONSTANT_ST = 0x1002,  // constant symbol
		S_UDT_ST = 0x1003,  // User defined type
		S_COBOLUDT_ST = 0x1004,  // special UDT for cobol that does not symbol pack
		S_MANYREG_ST = 0x1005,  // multiple register variable
		S_BPREL32_ST = 0x1006,  // BP-relative
		S_LDATA32_ST = 0x1007,  // Module-local symbol
		S_GDATA32_ST = 0x1008,  // Global data symbol
		S_PUB32_ST = 0x1009,  // a public symbol (CV internal reserved)
		S_LPROC32_ST = 0x100a,  // Local procedure start
		S_GPROC32_ST = 0x100b,  // Global procedure start
		S_VFTABLE32 = 0x100c,  // address of virtual function table
		S_REGREL32_ST = 0x100d,  // register relative address
		S_LTHREAD32_ST = 0x100e,  // local thread storage
		S_GTHREAD32_ST = 0x100f,  // global thread storage

		S_LPROCMIPS_ST = 0x1010,  // Local procedure start
		S_GPROCMIPS_ST = 0x1011,  // Global procedure start

		S_FRAMEPROC = 0x1012,  // extra frame and proc information
		S_COMPILE2_ST = 0x1013,  // extended compile flags and info

		// new symbols necessary for 16-bit enumerates of IA64 registers
		// and IA64 specific symbols

		S_MANYREG2_ST = 0x1014,  // multiple register variable
		S_LPROCIA64_ST = 0x1015,  // Local procedure start (IA64)
		S_GPROCIA64_ST = 0x1016,  // Global procedure start (IA64)

		// Local symbols for IL
		S_LOCALSLOT_ST = 0x1017,  // local IL sym with field for local slot index
		S_PARAMSLOT_ST = 0x1018,  // local IL sym with field for parameter slot index

		S_ANNOTATION = 0x1019,  // Annotation string literals

		// symbols to support managed code debugging
		S_GMANPROC_ST = 0x101a,  // Global proc
		S_LMANPROC_ST = 0x101b,  // Local proc
		S_RESERVED1 = 0x101c,  // reserved
		S_RESERVED2 = 0x101d,  // reserved
		S_RESERVED3 = 0x101e,  // reserved
		S_RESERVED4 = 0x101f,  // reserved
		S_LMANDATA_ST = 0x1020,
		S_GMANDATA_ST = 0x1021,
		S_MANFRAMEREL_ST = 0x1022,
		S_MANREGISTER_ST = 0x1023,
		S_MANSLOT_ST = 0x1024,
		S_MANMANYREG_ST = 0x1025,
		S_MANREGREL_ST = 0x1026,
		S_MANMANYREG2_ST = 0x1027,
		S_MANTYPREF = 0x1028,  // Index for type referenced by name from metadata
		S_UNAMESPACE_ST = 0x1029,  // Using namespace

		// Symbols w/ SZ name fields. All name fields contain utf8 encoded strings.
		S_ST_MAX = 0x1100,  // starting point for SZ name symbols

		S_OBJNAME = 0x1101,  // path to object file name
		S_THUNK32 = 0x1102,  // Thunk Start
		S_BLOCK32 = 0x1103,  // block start
		S_WITH32 = 0x1104,  // with start
		S_LABEL32 = 0x1105,  // code label
		S_REGISTER = 0x1106,  // Register variable
		S_CONSTANT = 0x1107,  // constant symbol
		S_UDT = 0x1108,  // User defined type
		S_COBOLUDT = 0x1109,  // special UDT for cobol that does not symbol pack
		S_MANYREG = 0x110a,  // multiple register variable
		S_BPREL32 = 0x110b,  // BP-relative
		S_LDATA32 = 0x110c,  // Module-local symbol
		S_GDATA32 = 0x110d,  // Global data symbol
		S_PUB32 = 0x110e,  // a public symbol (CV internal reserved)
		S_LPROC32 = 0x110f,  // Local procedure start
		S_GPROC32 = 0x1110,  // Global procedure start
		S_REGREL32 = 0x1111,  // register relative address
		S_LTHREAD32 = 0x1112,  // local thread storage
		S_GTHREAD32 = 0x1113,  // global thread storage

		S_LPROCMIPS = 0x1114,  // Local procedure start
		S_GPROCMIPS = 0x1115,  // Global procedure start
		S_COMPILE2 = 0x1116,  // extended compile flags and info
		S_MANYREG2 = 0x1117,  // multiple register variable
		S_LPROCIA64 = 0x1118,  // Local procedure start (IA64)
		S_GPROCIA64 = 0x1119,  // Global procedure start (IA64)
		S_LOCALSLOT = 0x111a,  // local IL sym with field for local slot index
		S_SLOT = S_LOCALSLOT,  // alias for LOCALSLOT
		S_PARAMSLOT = 0x111b,  // local IL sym with field for parameter slot index

		// symbols to support managed code debugging
		S_LMANDATA = 0x111c,
		S_GMANDATA = 0x111d,
		S_MANFRAMEREL = 0x111e,
		S_MANREGISTER = 0x111f,
		S_MANSLOT = 0x1120,
		S_MANMANYREG = 0x1121,
		S_MANREGREL = 0x1122,
		S_MANMANYREG2 = 0x1123,
		S_UNAMESPACE = 0x1124,  // Using namespace

		// ref symbols with name fields
		S_PROCREF = 0x1125,  // Reference to a procedure
		S_DATAREF = 0x1126,  // Reference to data
		S_LPROCREF = 0x1127,  // Local Reference to a procedure
		S_ANNOTATIONREF = 0x1128,  // Reference to an S_ANNOTATION symbol
		S_TOKENREF = 0x1129,  // Reference to one of the many MANPROCSYM's

		// continuation of managed symbols
		S_GMANPROC = 0x112a,  // Global proc
		S_LMANPROC = 0x112b,  // Local proc

		// short, light-weight thunks
		S_TRAMPOLINE = 0x112c,  // trampoline thunks
		S_MANCONSTANT = 0x112d,  // constants with metadata type info

		// native attributed local/parms
		S_ATTR_FRAMEREL = 0x112e,  // relative to virtual frame ptr
		S_ATTR_REGISTER = 0x112f,  // stored in a register
		S_ATTR_REGREL = 0x1130,  // relative to register (alternate frame ptr)
		S_ATTR_MANYREG = 0x1131,  // stored in >1 register

		// Separated code (from the compiler) support
		S_SEPCODE = 0x1132,

		S_LOCAL_2005 = 0x1133,  // defines a local symbol in optimized code
		S_DEFRANGE_2005 = 0x1134,  // defines a single range of addresses in which symbol can be evaluated
		S_DEFRANGE2_2005 = 0x1135,  // defines ranges of addresses in which symbol can be evaluated

		S_SECTION = 0x1136,  // A COFF section in a PE executable
		S_COFFGROUP = 0x1137,  // A COFF group
		S_EXPORT = 0x1138,  // A export

		S_CALLSITEINFO = 0x1139,  // Indirect call site information
		S_FRAMECOOKIE = 0x113a,  // Security cookie information

		S_DISCARDED = 0x113b,  // Discarded by LINK /OPT:REF (experimental, see richards)

		S_COMPILE3 = 0x113c,  // Replacement for S_COMPILE2
		S_ENVBLOCK = 0x113d,  // Environment block split off from S_COMPILE2

		S_LOCAL = 0x113e,  // defines a local symbol in optimized code
		S_DEFRANGE = 0x113f,  // defines a single range of addresses in which symbol can be evaluated
		S_DEFRANGE_SUBFIELD = 0x1140,           // ranges for a subfield

		S_DEFRANGE_REGISTER = 0x1141,           // ranges for en-registered symbol
		S_DEFRANGE_FRAMEPOINTER_REL = 0x1142,   // range for stack symbol.
		S_DEFRANGE_SUBFIELD_REGISTER = 0x1143,  // ranges for en-registered field of symbol
		S_DEFRANGE_FRAMEPOINTER_REL_FULL_SCOPE = 0x1144, // range for stack symbol span valid full scope of function body, gap might apply.
		S_DEFRANGE_REGISTER_REL = 0x1145, // range for symbol address as register + offset.

		// S_PROC symbols that reference ID instead of type
		S_LPROC32_ID = 0x1146,
		S_GPROC32_ID = 0x1147,
		S_LPROCMIPS_ID = 0x1148,
		S_GPROCMIPS_ID = 0x1149,
		S_LPROCIA64_ID = 0x114a,
		S_GPROCIA64_ID = 0x114b,

		S_BUILDINFO = 0x114c, // build information.
		S_INLINESITE = 0x114d, // inlined function callsite.
		S_INLINESITE_END = 0x114e,
		S_PROC_ID_END = 0x114f,

		S_DEFRANGE_HLSL = 0x1150,
		S_GDATA_HLSL = 0x1151,
		S_LDATA_HLSL = 0x1152,

		S_FILESTATIC = 0x1153,

		S_LOCAL_DPC_GROUPSHARED = 0x1154, // DPC groupshared variable
		S_LPROC32_DPC = 0x1155, // DPC local procedure start
		S_LPROC32_DPC_ID = 0x1156,
		S_DEFRANGE_DPC_PTR_TAG = 0x1157, // DPC pointer tag definition range
		S_DPC_SYM_TAG_MAP = 0x1158, // DPC pointer tag value to symbol record map

		S_ARMSWITCHTABLE = 0x1159,
		S_CALLEES = 0x115a,
		S_CALLERS = 0x115b,
		S_POGODATA = 0x115c,
		S_INLINESITE2 = 0x115d,      // extended inline site information

		S_HEAPALLOCSITE = 0x115e,    // heap allocation site

		S_MOD_TYPEREF = 0x115f,      // only generated at link time

		S_REF_MINIPDB = 0x1160,      // only generated at link time for mini PDB
		S_PDBMAP = 0x1161,      // only generated at link time for mini PDB

		S_GDATA_HLSL32 = 0x1162,
		S_LDATA_HLSL32 = 0x1163,

		S_GDATA_HLSL32_EX = 0x1164,
		S_LDATA_HLSL32_EX = 0x1165,

		S_RECTYPE_MAX,               // one greater than last
		S_RECTYPE_LAST = S_RECTYPE_MAX - 1,
		S_RECTYPE_PAD = S_RECTYPE_MAX + 0x100 // Used *only* to verify symbol record types so that current PDB code can potentially read
		// future PDBs (assuming no format change, etc).

	};

#pragma pack (push, 1)
	struct Slice
	{
		int offset;
		uint32_t size;
	};

	struct TpiHeader
	{
		uint32_t Version;
		uint32_t HeaderSize;
		uint32_t MinimumIndex;
		uint32_t MaximumIndex;
		uint32_t GprecSize;
		uint16_t TpiHashStream;
		uint16_t TpiHashPadStream;
		uint32_t HashKeySize;
		uint32_t HashBucketSize;
		Slice HashValues;
		Slice TiOff;
		Slice HashAdj;
	};

	struct DbiHeader
	{
		uint32_t Signature;
		uint32_t Version;
		uint32_t Age;
		int16_t GssymStream;
		uint16_t Vers;
		int16_t PssymStream;
		uint16_t PdbVersion;
		int16_t SymRecordStream;
		uint16_t PdbVersion2;
		uint32_t ModuleSize;
		uint32_t SecConSize;
		uint32_t SecMapSize;
		uint32_t FileInfoSize;
		uint32_t SrcModuleSize;
		uint32_t MfcIndex;
		uint32_t DbgHeaderSize;
		uint32_t EcInfoSize;
		uint16_t Flags;
		uint16_t Machine;
		uint32_t Reserved;
	};

	struct DbiDebugHeader
	{
		uint16_t FPO;
		uint16_t Exception;
		uint16_t Fixup;
		uint16_t OmapToSource;
		uint16_t OmapFromSource;
		uint16_t SectionHdr;
		uint16_t TokenRidMap;
		uint16_t XData;
		uint16_t PData;
		uint16_t NewFPO;
		uint16_t SectionHdrOriginal;
	};

	struct DbiSecCon
	{
		int16_t Section;
		uint16_t Padding1;
		int32_t Offset;
		uint32_t Size;
		uint32_t Flags;
		int16_t Module;
		uint16_t Pad2;
		uint32_t DataCrc;
		uint32_t RelocCrc;
	};

	struct DbiModuleInfo
	{
		int32_t Opened;
		DbiSecCon Sector;
		uint16_t Lags;
		uint16_t Stream;
		int32_t CbSyms;
		int32_t CbOldLines;
		int32_t CbLines;
		int16_t Files;
		int16_t Padding;
		uint32_t Offsets;
		int32_t NiSource;
		int32_t NiCompiler;

		// const char* ModuleName;
		// const char* ObjectName;
	};

	struct OMAP
	{
		uint32_t SourceAddress;
		uint32_t TargetAddress;

		bool operator<(const OMAP& Rhs) const
		{
			return this->SourceAddress < Rhs.SourceAddress;
		}
	};

	struct LfClass
	{
		//uint16_t Leaf;		 // LF_CLASS, LF_STRUCT, LF_INTERFACE
		uint16_t Count;          // count of number of elements in class
		uint16_t Property;       // property attribute field (prop_t)
		uint32_t Field;          // type index of LF_FIELD descriptor list
		uint32_t Derived;        // type index of derived from list if not zero
		uint32_t Vshape;         // type index of vshape table for this class
		//uint8_t Data[];  // data describing length of structure in
		// bytes and name
	};

	struct LfUnion 
	{
		//uint16_t  Leaf;           // LF_UNION
		uint16_t    Count;          // count of number of elements in class
		uint16_t    Property;       // property attribute field
		uint32_t    Field;          // type index of LF_FIELD descriptor list
		//uint8_t   Data[];         // variable length data describing length of
									// structure and name
	};

	struct LfClass32
	{
		//uint16_t Leaf;         // LF_CLASS, LF_STRUCT, LF_INTERFACE
		uint16_t Count;          // count of number of elements in class
		uint32_t Property;       // property attribute field (prop_t)
		uint32_t Field;          // type index of LF_FIELD descriptor list
		uint32_t Derived;        // type index of derived from list if not zero
		uint32_t Vshape;         // type index of vshape table for this class
		//unsigned char data[];  // data describing length of structure in
		// bytes and name
	};

	struct LfMember
	{
		//uint16_t Leaf;         // LF_MEMBER
		uint16_t Attr;           // attribute mask
		uint32_t Index;          // index of type record for field
		//uint8_t Offset[];         // offset inside struct
		// by length prefixed name of field
	};

	struct ProcSym32
	{
		// internal ushort reclen;    // Record length [SYMTYPE]
		// internal ushort rectyp;    // S_GPROC32 or S_LPROC32
		uint32_t Parent;     // pointer to the parent
		uint32_t End;        // pointer to this blocks end
		uint32_t Next;       // pointer to next symbol
		uint32_t Len;        // Proc length
		uint32_t DbgStart;   // Debug start offset
		uint32_t DbgEnd;     // Debug end offset
		uint32_t Typind;     // (type index) Type index
		uint32_t Off;
		uint16_t Seg;
		uint8_t Flags;      // (CV_PROCFLAGS) Proc flags
		//internal string name;       // Length-prefixed name
	};

	struct ThunkSym32
	{
		// internal ushort reclen;    // Record length [SYMTYPE]
		// internal ushort rectyp;    // S_THUNK32
		uint32_t Parent;     // pointer to the parent
		uint32_t End;        // pointer to this blocks end
		uint32_t Next;       // pointer to next symbol
		uint32_t Off;
		uint16_t Seg;
		uint16_t Len;        // length of thunk
		uint8_t Ord;        // THUNK_ORDINAL specifying type of thunk
		//internal string name;       // Length-prefixed name
		//internal byte[] variant;    // variant portion of thunk
	};

	struct PubSym32
	{
		//uint16_t Reclen;     // Record length
		//uint16_t Rectyp;     // S_PUB32
		uint32_t Pubsymflags;
		uint32_t Off;
		uint16_t Seg;
		//uint8_t Name[1];    // Length-prefixed name
	};

	struct DataSym32
	{
		//uint16_t Reclen;     // Record length
		//uint16_t Rectyp;     // S_LDATA32, S_GDATA32, S_LMANDATA, S_GMANDATA
		uint32_t Typind;     // Type index, or Metadata token if a managed symbol
		uint32_t Off;
		uint16_t Seg;
		//uint8_t Name[1];    // Length-prefixed name
	};

	struct LfVbClass
	{
		//uint16_t Leaf;         // LF_VBCLASS | LV_IVBCLASS
		uint16_t Attr;           // attribute
		uint32_t Index;          // type index of direct virtual base class
		uint32_t Vbptr;          // type index of virtual base pointer
		//char Vbpoff[0];		 // virtual base pointer offset from address point
								 // followed by virtual base offset from vbtable
	};

	struct LfBClass
	{
		//uint16_t Leaf;         // LF_BCLASS, LF_BINTERFACE
		uint16_t Attr;           // attribute
		uint32_t Index;          // type index of base class
		//char Offset[0];       // variable length offset of base within class
	};

	struct LfEnumerate
	{
		//uint16_t Leaf;     // LF_ENUMERATE
		uint16_t Attr;       // access
		//char Value[0];		 // variable length value field followed
							 // by length prefixed name
	};

	struct LfMethod
	{
		//uint16_t Leaf;         // LF_METHOD
		uint16_t Count;          // number of occurrences of function
		uint32_t MList;          // index to LF_METHODLIST record
		//char Name[0];         // length prefixed name of method
	};

	enum class MethodProperty : uint8_t
	{
		Vanilla = 0x00u,
		Virtual = 0x01u,
		Static = 0x02u,
		Friend = 0x03u,
		Intro = 0x04u,
		PureVirt = 0x05u,
		PureIntro = 0x06u
	};

	struct MemberAttributes
	{
		uint16_t Access : 2;			// access protection CV_access_t
		uint16_t Mprop : 3;				// method properties CV_methodprop_t
		uint16_t Pseudo : 1;			// compiler generated fcn and does not exist
		uint16_t Noinherit : 1;			// true if class cannot be inherited
		uint16_t Noconstruct : 1;		// true if class cannot be constructed
		uint16_t Compgenx : 1;			// compiler generated fcn and does exist
		uint16_t Sealed : 1;			// true if method cannot be overridden
		uint16_t Unused : 6;			// unused
	};

	struct LfOneMethod
	{
		//uint16_t Leaf;          // LF_ONEMETHOD
		MemberAttributes Attr;            // method attribute
		uint32_t Index;           // index to type record for procedure
		//char Vbaseoff[0];		  // offset in vfunctable if
								  // intro virtual followed by
								  // length prefixed name of method
	};

	struct LfNestTypeEx
	{
		//uint16_t Leaf;      // LF_NESTTYPEEX, LF_NESTTYPE
		uint16_t Attr;       // member access
		uint32_t Index;      // index of nested type definition
		//char Name[0];     // length prefixed type name
	};

	struct LfVFuncTab
	{
		//uint16_t Leaf;         // LF_VFUNCTAB
		uint16_t Pad0;           // internal padding, must be 0
		uint32_t Type;           // type index of pointer
	};

	struct LfIndex
	{
		//uint16_t Leaf;          // LF_INDEX
		uint16_t Pad0;           // internal padding, must be 0
		uint32_t Index;          // type index of referenced leaf
	};

	struct LfStMember
	{
		uint16_t Attr;
		uint32_t Index;			// index of type record for field
		//char Name[0];
	};

	struct LfBitfield
	{
		uint32_t	Type;           // type of bitfield
		uint8_t     Length;
		uint8_t     Position;
	};
#pragma pack ( pop )
}