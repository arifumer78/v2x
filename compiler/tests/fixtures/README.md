# fixtures/

Real ETSI ASN.1 modules, used as parser smoke-test input — not hand-crafted
snippets. `AsnParserSmokeTest` walks this whole tree recursively, so any
`.asn` file dropped in here gets exercised automatically. All pulled from
ETSI's own GitLab forge, `https://forge.etsi.org/rep/ITS/asn1/`, matching the
message set tracked in `../../../docs/messages/README.md`.

| File(s) | Message | Spec | Source repo (branch) |
|---|---|---|---|
| `CAM-PDU-Descriptions.asn` | CAM | TS 103 900 (R2) | `cam_ts103900` (`release2`) |
| `ETSI-ITS-CDD.asn` | — (Common Data Dictionary, imported by nearly everything else) | TS 102 894-2 | `cdd_ts102894_2` (`release2`) |
| `denm/DENM-PDU-Descriptions.asn` | DENM | TS 103 831 (R2) | `denm_ts103831` (`release2`) |
| `cpm/*.asn` (5 files) | CPM | TS 103 324 | `cpm_ts103324` (`master`) |
| `vam/*.asn` (2 files) | VAM | TS 103 300-3 | `vam-ts103300_3` (`master`) |
| `is/*.asn` (9 files: MAPEM/SPATEM/IVIM/SREM/SSEM/RTCMEM PDU descriptions + the DSRC/region modules they import from) | MAPEM, SPATEM, IVIM, SREM, SSEM, RTCMEM | TS 103 301 | `is_ts103301` (`release2`) |

MCM (TS 103 561) is deliberately not included — still a draft standard, not
in a stable ASN.1 release as of this writing (see `docs/messages/README.md`).

`ETSI-ITS-CDD.asn` was re-encoded from the source repo's Windows-1252 bytes
to valid UTF-8 (the source file has ~34 stray non-ASCII bytes, e.g. a
smart-quote apostrophe at line 570, that are not valid UTF-8 — a real
encoding mistake in ETSI's own file, not something introduced here). All
other files were valid UTF-8 as fetched.

All BSD-3-Clause licensed by ETSI — each subdirectory/file's `*-LICENSE`
was copied from its own source repo.
