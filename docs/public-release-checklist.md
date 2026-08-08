# Public release checklist

This checklist records the publication boundary and validation status of the final portfolio version.

## Repository structure and hygiene

- [x] Professional repository structure in place.
- [x] Generated Code Composer Studio build output excluded.
- [x] Historical archives and local launch/debug configuration excluded.
- [x] No tracked local user-home or cloud-sync paths found in the public-safe tree.
- [x] No original internal UTC technical PDFs included.
- [x] No UTC logo asset included.
- [x] No board-specific UTC register-map / low-level interface source included.
- [x] Automated `Public release guard` enabled in GitHub Actions.
- [x] Public-release guard passes on the current source tree.

## Attribution

- [x] Project identified as a two-student academic project.
- [x] Lorenzo Mazzante identified as `@lorenzomazzante`.
- [x] Martín Scorza identified as `@MartinScorza`.
- [x] Overall contribution recorded as **50/50**.
- [x] No unsupported subsystem-by-subsystem individual ownership claims.
- [x] Repository invitation sent to `@lorenzomazzante` by the repository owner.
- [ ] Lorenzo's accepted repository permission is visible through the GitHub integration. At the final audit, the API still reports `none`, which is consistent with an invitation that has not yet been accepted or propagated.

The collaborator acceptance status is not a technical publication blocker because the authorship and equal contribution are already documented accurately in the repository.

## Third-party and UTC material

- [x] Texas Instruments LCD support files retain their original license notices.
- [x] DFRobot project origin from the TI BOOSTXL-EDUMKII example is documented.
- [x] Internal UTC technical document excluded.
- [x] UTC register map / low-level protocol excluded.
- [x] Board-specific `Hand_*` low-level firmware excluded unless explicit provenance/redistribution evidence is obtained later.
- [x] UTC logo excluded.
- [x] Current decision: no repository-wide open-source license; `NOTICE.md` and `THIRD_PARTY_NOTICES.md` define the publication boundary.

## Software validation

- [x] Python interface dependency file included.
- [x] GitHub Actions installs declared Python dependencies.
- [x] `pip check` passes.
- [x] `pyserial` import passes.
- [x] Python GUI source passes `py_compile`.
- [x] Reconstructed the DFRobot MSP432P401R build in a clean environment with the documented TI legacy SDK/compiler.
- [x] Compiled the corrected DFRobot firmware from a fresh workspace/runner without user-specific absolute paths.
- [x] Linked a non-empty `dfrobot.out` image successfully.
- [x] No firmware compile/link warnings were observed in the successful build.

## DFRobot deterministic findings

Resolved and clean-build validated:

- [x] Initialize selector GPIO before reading the selector.
- [x] Resolve the P3/P7 selector configuration mismatch.
- [x] Correct malformed LCD mode/status format strings.
- [x] Clamp joystick mapping to its configured calibration range.
- [x] Clamp FSR mapping to its configured calibration range.

Broader architectural findings remain documented rather than silently rewritten:

- blocking progressive movement routines inside GPIO interrupt handling;
- lack of a robust FMA SPI timeout;
- lack of explicit button debounce;
- blocking UTC movement/communication behavior.

## Hardware evidence boundary

The current portfolio revision has not been newly flashed or re-tested on the original hardware. The original academic report/presentation remains the evidence for the physical demonstration.

The repository therefore states only that:

- [x] the original academic project was demonstrated on physical hardware;
- [x] the current DFRobot source is clean-build reproduced;
- [x] no new hardware performance measurements are claimed.

Additional oscilloscope, logic-analyzer or hardware re-test evidence is **not part of the final portfolio scope**.

## Recruiter-facing visual evidence

The final portfolio intentionally does not republish photos from the academic report and does not add new photos, GIFs or videos.

Instead, the repository provides original, rights-safe technical diagrams:

- [x] DFRobot architecture diagram;
- [x] UTC architecture diagram;
- [x] PI control-loop diagram;
- [x] recruiter-facing architecture links from the main README.

A detailed wiring/power schematic is intentionally omitted because the complete final external power/conditioning wiring cannot be verified from the preserved source material alone.

## Final publication review

- [x] Repository content is limited to intended public-safe material.
- [x] Python CI is green.
- [x] Public-release guard is green.
- [x] Critical deterministic DFRobot defects are corrected and clean-build validated.
- [x] README clearly distinguishes original physical demonstration from newly reproduced build evidence.
- [x] README contains no internal-work placeholders or stale validation statements identified during the final review.
- [x] Repository description is recruiter-readable and technically accurate.
- [x] Original architecture diagrams provide visual evidence without copying internal/vendor graphics.
- [x] Repository remains clearly described as an academic robotics prototype, not a clinically validated device.
- [ ] GitHub repository topics are configured. The current API reports no topics; recommended values are listed in the final audit.
- [ ] Repository visibility is intentionally changed from **Private** to **Public** when the owner is ready to publish it.

The two unchecked items above are GitHub metadata/visibility actions rather than missing technical content.
