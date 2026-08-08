# fixtures/

Real ETSI ASN.1 modules, used as parser smoke-test input — not hand-crafted
snippets. Pulled from ETSI's own GitLab forge:

- `CAM-PDU-Descriptions.asn` — ETSI TS 103 900 (Release 2 CAM), from
  https://forge.etsi.org/rep/ITS/asn1/cam_ts103900 (branch `release2`).
- `ETSI-ITS-CDD.asn` — ETSI TS 102 894-2 (Common Data Dictionary), from
  https://forge.etsi.org/rep/ITS/asn1/cdd_ts102894_2 (branch `release2`).
  Re-encoded from the source repo's Windows-1252 bytes to valid UTF-8 (the
  source file has ~34 stray non-ASCII bytes, e.g. a smart-quote apostrophe
  at line 570, that are not valid UTF-8 — this is a real encoding mistake in
  ETSI's own file, not something introduced here).

Both are BSD-3-Clause licensed by ETSI — see `CAM-LICENSE` / `CDD-LICENSE`
(copied from the same source repos).
