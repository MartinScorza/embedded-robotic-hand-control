# Public release checklist

This checklist tracks the minimum evidence and governance required before changing this portfolio repository from private to public.

## Repository structure and hygiene

- [x] Professional repository structure in place.
- [x] Generated Code Composer Studio build output excluded.
- [x] Historical archives and local launch/debug configuration excluded.
- [x] No tracked absolute `C:/Users/...` or OneDrive paths found in the public-safe tree.
- [x] No original internal UTC technical PDFs included.
- [x] No UTC logo asset included.
- [x] No board-specific UTC register-map / low-level interface source included.
- [x] Automated `Public release guard` enabled in GitHub Actions.
- [x] First public-release guard execution passed.

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
- [ ] Reconstruct a clean MSP432P401R project with the documented TI toolchain.
- [ ] Compile the DFRobot firmware from a clean workspace.
- [ ] Record compiler warnings/errors.

## Firmware findings requiring review

Tracked in Issue #3:

- [ ] Initialize selector GPIO before reading the selector.
- [ ] Resolve the P3/P7 selector configuration mismatch.
- [ ] Correct remaining malformed LCD format strings.
- [ ] Clamp joystick mapping to its configured calibration range.
- [ ] Clamp FSR mapping to its configured calibration range.

Broader architectural findings that should not be silently rewritten without hardware validation:

- blocking progressive movement routines inside GPIO interrupt handling;
- lack of a robust FMA SPI timeout;
- lack of explicit button debounce;
- blocking UTC movement/communication behavior.

## Hardware evidence

- [ ] Confirm the DFRobot selector wiring used in the final setup.
- [ ] Verify five PWM outputs on hardware if the setup is still available.
- [ ] Verify representative 50 Hz / 1000–1600 µs servo commands if hardware is available.
- [ ] Re-test joystick, FSR and FMA operating modes after any firmware fixes.
- [ ] Keep UTC control-loop frequency described as nominal 200 Hz unless it is measured end-to-end.

## Recruiter-facing media

Tracked in Issue #6.

Minimum desirable set before public release:

- [ ] one short overview GIF/video showing the project hardware;
- [ ] one clean photo of the DFRobot setup;
- [ ] one clean public-safe photo or demo of the UTC-hand setup if available;
- [ ] one clean Python GUI screenshot;
- [ ] original architecture diagrams or equivalent recruiter-readable visual explanation.

If original hardware/media are no longer available, the repository may still be released using the documented original demonstration evidence, but it must not imply that new hardware validation was performed.

## Final publication review

Before changing visibility to public:

- [ ] `main` contains only intended public-safe material.
- [ ] Python CI is green.
- [ ] Public-release guard is green.
- [ ] Open Critical items are reviewed and accurately reflected in the README.
- [ ] README contains no `[TO CONFIRM]` placeholders intended for internal work.
- [ ] Repository description and topics are set for embedded systems / robotics / biomedical-engineering portfolio discovery.
- [ ] Repository remains clearly described as an academic robotics prototype, not a clinically validated device.
- [ ] Final visibility change to **Public** is intentional.
