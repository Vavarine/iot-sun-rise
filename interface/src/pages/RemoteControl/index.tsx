import sunFilledIcon from "../../assets/sun-filled.svg?raw";
import sunIcon from "../../assets/sun.svg?raw";
import { COLOR_BUTTONS } from "../../constants/COLOR_BUTTONS";

export function RemoteControl() {
  const handleButtonClick = async (e: Event) => {
    const irCode = (e.currentTarget as HTMLElement).dataset.ir;
    console.log(irCode);

    const data = {
      code: parseInt(irCode),
    };

    await fetch("https://sunrise.evailson.dev/api/ir-send", {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify(data),
    });
  };

  return (
    <div class="container py-8 flex flex-col justify-center items-center">
      <div class="grid grid-cols-4 gap-2 w-fit">
        <div class="col-start-1 col-span-3 grid grid-cols-subgrid">
          <button
            class="btn btn-circle col-start-1"
            dangerouslySetInnerHTML={{ __html: sunFilledIcon }}
            onClick={handleButtonClick}
            data-ir="0xFF00FF"
          />
          <button
            class="btn btn-circle col-start-3"
            dangerouslySetInnerHTML={{ __html: sunIcon }}
            onClick={handleButtonClick}
            data-ir="0xFF40BF"
          />
        </div>
        <div class="col-start-4 row-start-1 row-span-7 grid grid-rows-subgrid">
          <button class="btn btn-circle uppercase" onClick={handleButtonClick} data-ir="0xFFC03F">
            w
          </button>
          <button class="btn btn-circle row-start-4" onClick={handleButtonClick} data-ir="0xFFF00F">
            Fla
          </button>
          <button class="btn btn-circle row-start-5" onClick={handleButtonClick} data-ir="0xFFC837">
            Str
          </button>
          <button class="btn btn-circle row-start-6" onClick={handleButtonClick} data-ir="0xFFE817">
            Smo
          </button>
          <button class="btn btn-circle row-start-7" onClick={handleButtonClick} data-ir="0xFFD827">
            Mod
          </button>
        </div>
        <div class="col-start-1 col-span-4 row-start-2 row-span-1 grid grid-cols-subgrid">
          <button
            class="btn btn-circle col-start-3 uppercase"
            onClick={handleButtonClick}
            data-ir="0xFF609F"
          >
            Off
          </button>
          <button
            class="btn btn-circle col-start-4 uppercase"
            onClick={handleButtonClick}
            data-ir="0xFFE01F"
          >
            On
          </button>
        </div>
        {COLOR_BUTTONS.map((colorButton, i) => (
          <button
            class="btn btn-circle"
            style={{
              backgroundColor: colorButton.color,
            }}
            onClick={handleButtonClick}
            data-ir={colorButton.irData}
          />
        ))}
      </div>
    </div>
  );
}
