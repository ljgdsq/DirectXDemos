struct VsOut {
    float3 color : Color;
    float4 pos : SV_Position;
};

cbuffer CBuf
{
    row_major matrix transform;
};

VsOut main(float2 pos : Position,float4 color:Color)
{
    VsOut out1;
    out1.pos =mul( float4(pos.x, pos.y, 0.0f, 1.0f), transform);
    out1.color = color;
    return out1;
}