# Public release checklist

This checklist tracks the minimum evidence and governance required before changing this portfolio repository from private to public.

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
- [ ] Add `@lorenzomazzante` as repository collaborator with write access.

## Third-party and UTC material

- [x] Texas Instruments LCD support files retain their original license notices.
- [x] DFRobot project origin from the TI BOOSTXL-EDUMKII example is documented.
- [x] Internal UTC technical document excluded.
- [x] UTC register map / low-level protocol excluded.
- [x] Board-specific `Hand_*` low-level firmware excluded pending explicit provenance/redistribution evidence.
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

## Hardware evidence

The current portfolio revision has not been newly flashed/re-tested on the original hardware. The original academic documentation remains the evidence for physical operation.

Optional new validation if the hardware is still available:

- [ ] confirm the DFRobot selector wiring used in the final setup;
- [ ] verify five PWM outputs on hardware;
- [ ] verify representative 50 Hz / 1000–1600 µs servo commands;
- [ ] re-test joystick, FSR and FMA modes after the source fixes;
- [ ] keep UTC control-loop frequency described as nominal 200 Hz unless it is measured end-to-end.

Lack of new physical access does not invalidate the portfolio release as long as the README keeps the distinction between the original hardware demonstration and the newly reproduced clean build.

## Recruiter-facing media

Tracked in Issue #6.

Minimum desirable set before public release:

- [ ] one short overview GIF/video showing the project hardware, if original footage is available;
- [ ] one clean photo of the DFRobot setup, if available;
- [ ] one clean public-safe photo or demo of the UTC-hand setup, if available;
- [ ] one clean Python GUI screenshot;
- [ ] original architecture diagrams or equivalent recruiter-readable visual explanation.

The reviewed project archives do not contain original demo photos/videos apart from the excluded UTC logo. If external original media are unavailable, the repository may still be released with original/redrawn architecture diagrams and the documented original demonstration evidence.

## Final publication review

Before changing visibility to public:

- [x] `main` contains only intended public-safe material.
- [x] Python CI is green.
- [x] Public-release guard is green.
- [x] Critical deterministic DFRobot defects identified in Issue #3 are corrected and clean-build validated.
- [x] README clearly distinguishes original physical demonstration from newly reproduced build evidence.
- [ ] README contains no internal-work placeholders or stale validation statements.
- [ ] Repository description and topics are finalized for embedded systems / robotics / biomedical-engineering portfolio discovery.
- [ ] Lorenzo has repository access or has otherwise been informed/shared the repository as agreed by the team.
- [ ] Recruiter-facing visual evidence is added where available.
- [x] Repository remains clearly described as an academic robotics prototype, not a clinically validated device.
- [ ] Final visibility change to **Public** is intentional.
