#include "fd_shred.h"

FD_STATIC_ASSERT( __builtin_offsetof( fd_shred_t, signature       )==0x00, unit_test );
FD_STATIC_ASSERT( __builtin_offsetof( fd_shred_t, variant         )==0x40, unit_test );
FD_STATIC_ASSERT( __builtin_offsetof( fd_shred_t, slot            )==0x41, unit_test );
FD_STATIC_ASSERT( __builtin_offsetof( fd_shred_t, idx             )==0x49, unit_test );
FD_STATIC_ASSERT( __builtin_offsetof( fd_shred_t, version         )==0x4d, unit_test );
FD_STATIC_ASSERT( __builtin_offsetof( fd_shred_t, fec_set_idx     )==0x4f, unit_test );

FD_STATIC_ASSERT( __builtin_offsetof( fd_shred_t, code.data_cnt   )==0x53, unit_test );
FD_STATIC_ASSERT( __builtin_offsetof( fd_shred_t, code.code_cnt   )==0x55, unit_test );
FD_STATIC_ASSERT( __builtin_offsetof( fd_shred_t, code.idx        )==0x57, unit_test );

FD_STATIC_ASSERT( __builtin_offsetof( fd_shred_t, data.parent_off )==0x53, unit_test );
FD_STATIC_ASSERT( __builtin_offsetof( fd_shred_t, data.flags      )==0x55, unit_test );
FD_STATIC_ASSERT( __builtin_offsetof( fd_shred_t, data.size       )==0x56, unit_test );

static uchar const fixture_legacy_coding_shred[FD_SHRED_MAX_SZ] = {
  0x12,0x5f,0x43,0x31,0xbe,0x64,0x52,0x07,0x88,0xc8,0x2b,0x22,0x71,0x4a,0xb9,0x2c,
  0xa9,0x46,0xfb,0x2f,0xc6,0x50,0xc6,0xf8,0x30,0x23,0x4e,0x1e,0x96,0x76,0xf3,0xab,
  0x6f,0xf1,0x7c,0x41,0x2e,0xe7,0x39,0x37,0x6b,0x37,0x94,0x60,0x14,0x92,0x65,0x4e,
  0x55,0xdc,0x3a,0x80,0x6a,0xc7,0x39,0x53,0x61,0xe5,0xfc,0x57,0x5b,0x67,0x54,0x00,
  0x5a,0x6d,0xe9,0x75,0x08,0x00,0x00,0x00,0x00,0x7a,0x5b,0x00,0x00,0xc2,0xb8,0x0b,
  0x53,0x00,0x00,0x20,0x00,0x3a,0x00,0x2b,0x00,0x86,0xa8,0x40,0x05,0x68,0xa4,0xc4,
  0xdd,0xa5,0x55,0x8e,0x4b,0x4e,0x87,0xd4,0x26,0x59,0xad,0x77,0xfa,0xc8,0xbf,0xb2,
  0xb6,0x76,0x97,0x36,0x01,0xef,0xa2,0x3d,0xa4,0x28,0x77,0x4f,0x78,0x21,0xe3,0x26,
  0xcd,0x14,0x4d,0x4c,0x96,0x3d,0x39,0x3d,0xde,0xb3,0xbb,0x2a,0x78,0x23,0x5c,0x13,
  0x88,0x89,0xb4,0x65,0xfb,0x4d,0x6b,0x7d,0xbc,0xe4,0xd1,0xdd,0x64,0xe8,0x06,0x17,
  0x72,0x29,0x20,0x8f,0x32,0x89,0xe8,0xe1,0x43,0x8a,0xfb,0xf1,0xd0,0x02,0x12,0xd3,
  0xf5,0xa2,0xeb,0xda,0xee,0x9d,0xb3,
};

static uchar const fixture_legacy_data_shred[FD_SHRED_MAX_SZ] = {
  0xfb,0x2b,0x54,0xe1,0xe8,0x81,0x25,0xb2,0xde,0xe1,0x9a,0xd4,0x94,0x41,0x1c,0x31,
  0xeb,0xc2,0x3a,0xc4,0x85,0xfa,0x1b,0x8c,0x22,0x69,0x18,0xcb,0xf6,0x6d,0xac,0x25,
  0x73,0xd0,0x1c,0x76,0x14,0xfa,0x1f,0x2c,0x94,0xd2,0xa2,0x13,0x42,0xee,0x8f,0x33,
  0xd8,0xaf,0xff,0x1d,0xaa,0x4c,0x40,0xc1,0xd3,0xda,0x89,0xff,0x51,0xb6,0x3a,0x05,
  0xa5,0x92,0xd3,0x75,0x08,0x00,0x00,0x00,0x00,0x0d,0x70,0x00,0x00,0x85,0xb0,0xf1,
  0x6f,0x00,0x00,0x26,0x8e,0xe5,0x73,0x04,0x3f,0x84,0x53,0x6a,0xdc,0x97,0x9d,0x0d,
  0x84,0xda,0x38,0x43,0x9d,0xf0,0xdb,0x16,0xb1,0xa9,0x0a,0x45,0xb2,0xed,0x91,0x74,
  0x2e,0xd4,0xb9,0x62,0x97,0x7d,0x57,0x62,0x56,0xac,0xb3,0xc0,0xf9,0x0d,0x46,0x2f,
  0x89,0x23,0x6f,0xe3,0xfe,0x3b,0xe7,0x31,0x66,0x35,0x89,0xe6,0xd1,0x7b,0x65,0xe2,
  0x64,0x17,0xa8,0x0e,0x2f,0x4f,0x9d,0x63,0x2a,0xea,0xe9,0x73,0x72,0x46,0xda,0x44,
  0xc4,0xc1,0x3a,0x30,0x08,0x83,0x04,0xbf,0xd4,0x52,0xb0,0xd9,0xc9,0xff,0x61,0xa8,
  0x6b,0x51,0x6a,0x54,0x9b,0xcf,0x39,
};

static uchar const fixture_legacy_data_shred_empty[FD_SHRED_MAX_SZ] = {
  0x61,0x57,0xad,0x27,0x31,0xa7,0xf6,0x15,0x29,0xd2,0x29,0x6a,0x88,0xad,0xd9,0x81,
  0x84,0xb7,0xdf,0x1c,0x7f,0x92,0x45,0x12,0x0f,0x42,0x31,0x14,0xcf,0xaa,0xe1,0x6d,
  0x89,0xd0,0x12,0x0e,0x70,0x57,0xc0,0xad,0x65,0x27,0xc3,0x68,0x44,0x10,0x0f,0x90,
  0x1b,0xa8,0x75,0x4a,0x03,0x96,0x0a,0x5d,0xeb,0xe1,0xea,0xf1,0x6b,0x44,0x34,0x01,
  0xa5,0x6a,0xe9,0x77,0x08,0x00,0x00,0x00,0x00,0xc3,0x53,0x00,0x00,0x35,0xe8,0xa6,
  0x53,0x00,0x00,0x4f,0xc8,0x71,0x58,0x00,
};

int
main( int     argc,
      char ** argv ) {
  fd_boot( &argc, &argv );

  fd_shred_t const * shred;

  /* Test shred parsing rules. */
  for( uint i = 0; i < 0x100U; i++ ) {
    /* Create fake shred */
    uchar buf[FD_SHRED_MAX_SZ] = {0};
    buf[0x40] = (uchar)i;

    /* Make sure our buffer is larger than the smallest size of the largest shred variant. */
    FD_STATIC_ASSERT( sizeof(buf) > sizeof(fd_shred_t)+(((1<<4)-1))*FD_SHRED_MERKLE_NODE_SZ+FD_SHRED_MERKLE_ROOT_SZ,
                      "buffer must be large enough to fit shred with 15 merkle nodes" );

    /* Test type detection */
    int is_legacy  =  i      ==0x5a ||  i      ==0xa5;
    int is_merkle  = (i&0xf0)==FD_SHRED_TYPE_MERKLE_CODE || (i&0xf0)==FD_SHRED_TYPE_MERKLE_DATA
      || (i&0xf0)==FD_SHRED_TYPE_MERKLE_DATA_CHAINED || (i&0xf0)==FD_SHRED_TYPE_MERKLE_CODE_CHAINED
      || (i&0xf0)==FD_SHRED_TYPE_MERKLE_DATA_CHAINED_RESIGNED || (i&0xf0)==FD_SHRED_TYPE_MERKLE_CODE_CHAINED_RESIGNED;
    int is_data    =  i      ==0xa5 || (i&0xf0)==FD_SHRED_TYPE_MERKLE_DATA  || (i&0xf0)==FD_SHRED_TYPE_MERKLE_DATA_CHAINED || (i&0xf0)==FD_SHRED_TYPE_MERKLE_DATA_CHAINED_RESIGNED;
    int is_code    =  i      ==0x5a || (i&0xf0)==FD_SHRED_TYPE_MERKLE_CODE  || (i&0xf0)==FD_SHRED_TYPE_MERKLE_CODE_CHAINED || (i&0xf0)==FD_SHRED_TYPE_MERKLE_CODE_CHAINED_RESIGNED;
    int is_resigned = ((i&0xf0)==FD_SHRED_TYPE_MERKLE_DATA_CHAINED_RESIGNED || (i&0xf0)==FD_SHRED_TYPE_MERKLE_CODE_CHAINED_RESIGNED );
    int is_chained = ((i&0xf0)==FD_SHRED_TYPE_MERKLE_DATA_CHAINED || (i&0xf0)==FD_SHRED_TYPE_MERKLE_CODE_CHAINED ) || is_resigned;
    int is_valid   = ((is_legacy^is_merkle) && (is_data^is_code) && (!is_chained || is_merkle)) || (is_resigned && is_chained && !is_legacy);

    /* Find sizes for shred type */
    ulong header_sz = 0;
    if( FD_LIKELY( is_data   ) ) header_sz = FD_SHRED_DATA_HEADER_SZ;
    if( FD_LIKELY( is_code   ) ) header_sz = FD_SHRED_CODE_HEADER_SZ;
    ulong merkle_sz = 0;
    if( FD_LIKELY( is_merkle ) ) merkle_sz = (i&0x0f)*FD_SHRED_MERKLE_NODE_SZ;
    ulong chained_root_sz = 0;
    if( FD_LIKELY( is_chained ) ) chained_root_sz = FD_SHRED_MERKLE_ROOT_SZ;
    ulong resigned_sz = 0;
    if( FD_LIKELY( is_resigned ) ) resigned_sz = FD_SHRED_SIGNATURE_SZ;
    ulong payload_sz = ((is_merkle&is_data) ? FD_SHRED_MIN_SZ : FD_SHRED_MAX_SZ) - header_sz - merkle_sz - chained_root_sz - resigned_sz;

    if( is_data )
      *(ushort*)(buf+0x56) = (ushort)(payload_sz + header_sz); /* write data.size */

    /* Test type-specific bounds checks */
    if( FD_LIKELY( is_valid )) {
      /* Test merkle node count */
      uint merkle_cnt = fd_shred_merkle_cnt( (uchar)i );
      FD_TEST( (is_merkle || merkle_cnt==0) && merkle_cnt<=16 );

      FD_TEST( header_sz > 0 );
      FD_TEST( !merkle_sz || is_merkle ); /* !is_merkle implies merkle_sz == 0 */

      FD_LOG_NOTICE(( "shred variant 0x%02x: type=%s header_sz=0x%lx merkle_sz=0x%lx payload_sz=0x%lx",
                      i, is_data ? "data" : "code", header_sz, merkle_sz, payload_sz ));

      /* Test with min buffer size */
      fd_shred_t const * shred;
      FD_TEST( (shred = fd_shred_parse( buf, sizeof(buf) ))!=NULL );
      FD_TEST( i==fd_shred_variant( fd_shred_type( shred->variant ), (uchar)fd_shred_merkle_cnt( shred->variant ) ) );
      FD_TEST( fd_shred_header_sz ( shred->variant )==header_sz  );
      FD_TEST( fd_shred_payload_sz( shred          )==payload_sz );
      FD_TEST( fd_shred_merkle_sz ( shred->variant )==merkle_sz  );

      FD_TEST( is_data    ==fd_shred_is_data(     fd_shred_type( shred->variant ) ) );
      FD_TEST( is_code    ==fd_shred_is_code(     fd_shred_type( shred->variant ) ) );
      FD_TEST( is_chained ==fd_shred_is_chained ( fd_shred_type( shred->variant ) ) );
      FD_TEST( is_resigned==fd_shred_is_resigned( fd_shred_type( shred->variant ) ) );
    } else {
      FD_LOG_NOTICE(( "shred type 0x%02x: invalid", i ));
      /* Invalid shred types should always be rejected irrespective of buffer size */
      FD_TEST( fd_shred_parse( buf, sizeof(buf) )==NULL );
    }
  }

# define PARSE(x) fd_shred_parse( x, sizeof(x) )

  /* Parse legacy data shred. */
  shred = PARSE( fixture_legacy_data_shred );
  FD_TEST( shred != NULL );
  FD_TEST( !memcmp( shred->signature, fixture_legacy_data_shred, sizeof(shred->signature) ) );
  FD_TEST( fd_shred_type      ( shred->variant )==FD_SHRED_TYPE_LEGACY_DATA );
  FD_TEST( fd_shred_merkle_cnt( shred->variant )==0 );
  FD_TEST( shred->slot           ==141939602 );
  FD_TEST( shred->idx            ==    28685 );
  FD_TEST( shred->version        ==    45189 );
  FD_TEST( shred->fec_set_idx    ==    28657 );
  FD_TEST( shred->data.parent_off==    36390 );
  FD_TEST( shred->data.flags     ==     0xe5 );

  /* Parse empty legacy data shred. */
  shred = PARSE( fixture_legacy_data_shred_empty );
  FD_TEST( shred != NULL );
  FD_TEST( !memcmp( shred->signature, fixture_legacy_data_shred_empty, sizeof(shred->signature) ) );
  FD_TEST( fd_shred_type      ( shred->variant )==FD_SHRED_TYPE_LEGACY_DATA );
  FD_TEST( fd_shred_merkle_cnt( shred->variant )==0 );
  FD_TEST( shred->slot           ==142076266 );
  FD_TEST( shred->idx            ==    21443 );
  FD_TEST( shred->version        ==    59445 );
  FD_TEST( shred->fec_set_idx    ==    21414 );
  FD_TEST( shred->data.parent_off==    51279 );
  FD_TEST( shred->data.flags     ==     0x71 );
  FD_TEST( shred->data.size      ==     0x58 );

  /* Parse legacy coding shred. */
  shred = PARSE( fixture_legacy_coding_shred );
  FD_TEST( shred != NULL );
  FD_TEST( !memcmp( shred->signature, fixture_legacy_coding_shred, sizeof(shred->signature) ) );
  FD_TEST( fd_shred_type      ( shred->variant )==FD_SHRED_TYPE_LEGACY_CODE );
  FD_TEST( fd_shred_merkle_cnt( shred->variant )==0 );
  FD_TEST( shred->slot         == 141945197 );
  FD_TEST( shred->idx          ==     23418 );
  FD_TEST( shred->version      ==     47298 );
  FD_TEST( shred->fec_set_idx  ==     21259 );
  FD_TEST( shred->code.data_cnt==        32 );
  FD_TEST( shred->code.code_cnt==        58 );
  FD_TEST( shred->code.idx     ==        43 );

# undef PARSE
  FD_TEST( fd_shred_swap_type( FD_SHRED_TYPE_LEGACY_DATA                  )==FD_SHRED_TYPE_LEGACY_CODE                  );
  FD_TEST( fd_shred_swap_type( FD_SHRED_TYPE_LEGACY_CODE                  )==FD_SHRED_TYPE_LEGACY_DATA                  );
  FD_TEST( fd_shred_swap_type( FD_SHRED_TYPE_MERKLE_DATA                  )==FD_SHRED_TYPE_MERKLE_CODE                  );
  FD_TEST( fd_shred_swap_type( FD_SHRED_TYPE_MERKLE_CODE                  )==FD_SHRED_TYPE_MERKLE_DATA                  );
  FD_TEST( fd_shred_swap_type( FD_SHRED_TYPE_MERKLE_DATA_CHAINED          )==FD_SHRED_TYPE_MERKLE_CODE_CHAINED          );
  FD_TEST( fd_shred_swap_type( FD_SHRED_TYPE_MERKLE_CODE_CHAINED          )==FD_SHRED_TYPE_MERKLE_DATA_CHAINED          );
  FD_TEST( fd_shred_swap_type( FD_SHRED_TYPE_MERKLE_DATA_CHAINED_RESIGNED )==FD_SHRED_TYPE_MERKLE_CODE_CHAINED_RESIGNED );
  FD_TEST( fd_shred_swap_type( FD_SHRED_TYPE_MERKLE_CODE_CHAINED_RESIGNED )==FD_SHRED_TYPE_MERKLE_DATA_CHAINED_RESIGNED );


  FD_LOG_NOTICE(( "pass" ));
  fd_halt();
  return 0;
}
