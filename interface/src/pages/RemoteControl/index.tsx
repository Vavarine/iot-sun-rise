import sunFilledIcon from "../../assets/sun-filled.svg?raw";
import sunIcon from "../../assets/sun.svg?raw";
import { COLOR_BUTTONS } from "../../constants/COLOR_BUTTONS";

export function RemoteControl() {
  return (
    <div class="container py-8 flex flex-col justify-center items-center">
      <div class="grid grid-cols-4 gap-2 w-fit">
        <div class="col-start-1 col-span-3 grid grid-cols-subgrid">
          <button
            class="btn btn-circle col-start-1"
            dangerouslySetInnerHTML={{ __html: sunFilledIcon }}
          />
          <button
            class="btn btn-circle col-start-3"
            dangerouslySetInnerHTML={{ __html: sunIcon }}
          />
        </div>
        <div class="col-start-4 row-start-1 row-span-7 grid grid-rows-subgrid">
          <button class="btn btn-circle uppercase">w</button>
          <button class="btn btn-circle row-start-4 ">Fla</button>
          <button class="btn btn-circle row-start-5 ">Str</button>
          <button class="btn btn-circle row-start-6 ">Smo</button>
          <button class="btn btn-circle row-start-7 ">Mod</button>
        </div>
        <div class="col-start-1 col-span-4 row-start-2 row-span-1 grid grid-cols-subgrid">
          <button class="btn btn-circle col-start-3 uppercase">Off</button>
          <button class="btn btn-circle col-start-4 uppercase">On</button>
        </div>
        {COLOR_BUTTONS.map((colorButton, i) => (
          <button
            class="btn btn-circle"
            style={{
              backgroundColor: colorButton.color,
            }}
          />
        ))}
      </div>
    </div>
  );
}
